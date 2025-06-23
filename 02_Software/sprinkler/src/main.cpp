#include <Arduino.h>
#include <pin_config.h>
#include <sleep_manager.h>
#include <hw_abstraction.h>
#include <sw_timer.h>
#include <logger.h>
#include <display.h>
#include <pump_manager.h>

#include <rtc_manager.h>
#include <daily_timer.h>

#define DEVELOPMENT 1
#define PRODUCTION 2

#if BUILD_TYPE == PRODUCTION
#include <nvs_flash.h>
#endif

#define SWVERSION "0.1.0"

/******************************************************************************
                                GLOBAL VARIABLES
******************************************************************************/

DisplayManager myDisplay;
RTC_DS3231 rtc;
RtcManager rtcManager(rtc);

constexpr int PUMP1_ID = 1;
constexpr int PUMP2_ID = 2;

DailyTimer pump1_timer(PUMP1_ID, rtc);
DailyTimer pump2_timer(PUMP2_ID, rtc);

/******************************************************************************
                                    FUNCTIONS
******************************************************************************/
void manual_handler();
void handle_pump_line(DailyTimer &timer, PumpManager &pump, PumpManager &other_pump, int pump_id);
void timer_activation_handler(DailyTimer &p1_timer, DailyTimer &p2_timer, PumpManager &p1, PumpManager &p2);
void display_handler();
void monitor_tank_emptiness(PumpManager &pump);

/******************************************************************************
                                      SETUP
******************************************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println(SWVERSION);

  // Remove persisted variables if we flash for production
  // TODO find a way to run this only once.
  /*
  #if BUILD_TYPE == PRODUCTION
    Serial.println("Erasing persistency...");
    nvs_flash_erase();
    nvs_flash_init();
    Serial.println("Persistency erased.");
  #endif
  */

  setup_all_gpio();
  debugln("M-GPIO initialized");

  // Timer related initialization
  systemTimeHandler.wakeupTimestamp = millis();

#if BUILD_TYPE == DEVELOPMENT
  systemTimeHandler.deepSleepWakeupAfterMinutes = 1;
#endif

  // setupDeepSleepWakeupAfterMins(systemTimeHandler.deepSleepWakeupAfterMinutes);
  // debugln("M-Deep spleep setup completed.");

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1:
    Buttons::DEEP_SLEEP_BUTTON.pressed = true;
    debugln("M-Wakeup due to button");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    debugln("M-Wakeup caused by timer");
    break;
  default:
    debugln("M-Standard wakeup");
  }

  Wire.begin();
  debugln("M-I2C setup done.");

  rtc.begin();
  // We are going to run, deactivate all alarms to avoid unexpected shutdown.
  rtcManager.deactivateAlarm();
  debugln("M-Setup RTC done.");

  if (myDisplay.begin())
  {
    myDisplay.main_screen(rtc.now());
    delay(100);
  }

#if BUILD_TYPE == DEVELOPMENT
  // pump1_timer.reset_persistency();
  // pump2_timer.reset_persistency();
#endif

  pump1_timer.begin();
  pump2_timer.begin();

  debugln("M-Setup pump Timers done.");

  myDisplay.update_pump_timer(1, pump1_timer);
  myDisplay.update_pump_timer(2, pump2_timer);

  setup_water_sensor_enable();
  setup_water_sensor_signal_input();
  setup_display_enable();
}

/******************************************************************************
                                  MAIN LOOP
******************************************************************************/

void loop()
{

  DateTime now = rtc.now();
  myDisplay.update_time(now);
  // myDisplay.update_pump_timer(1, pump1_timer);
  // myDisplay.update_pump_timer(2, pump2_timer);
  //  debugln();
  debugln(now.timestamp());

  if (Buttons::RESET_BUTTON.pressed)
  {
    debugln("Reset operations");
    ESP.restart();
  }

  if (systemTimeHandler.isWakeupTimeExpired())
  {
    debug("M-System wakeup expire after: ");
    debug(systemTimeHandler.minutesWakeupPeriod);
    debugln(" minute(s).");
    debug("M-Entering Deep Sleep for: ");
    debug(systemTimeHandler.deepSleepWakeupAfterMinutes);
    debugln(" minute(s).");
    myDisplay.sleep_screen();
    rtcManager.activateAlarm();
  }

  manual_handler();
  timer_activation_handler(pump1_timer, pump2_timer, pump1, pump2);
  display_handler();
  monitor_tank_emptiness(pump1);
  monitor_tank_emptiness(pump2);

  debug("Water sensor. Digital: ");
  int value = digitalRead(Constants::WATER_SENSOR_SIG);
  debug(value);
  debug(" Analog: ");
  debug(analogRead(Constants::WATER_SENSOR_SIG));
  debug(" TankEmpty?: ");
  debugln(TankSensor::isTankEmpty());

  debug("Pump1 empty? ");
  debugln(pump1.is_pump_tank_empty());
  debug("Pump2 empty? ");
  debugln(pump2.is_pump_tank_empty());


  delay(100);
}

/******************************************************************************
                            FUNCTIONS DEFINITION
******************************************************************************/

void manual_handler()
{
  if (Buttons::SELECT_LINE_BUTTON.pressed)
  {
    Buttons::SELECT_LINE_BUTTON.pressed = false;
    myDisplay.change_line();
    debugln("M-Line changed");
  }

  if (myDisplay.selected_line == myDisplay.time_line)
  {
    if (Buttons::HOUR_BUTTON.pressed)
    {
      Buttons::HOUR_BUTTON.pressed = false;
      rtcManager.increaseOneHour();
    }
    if (Buttons::MIN_BUTTON.pressed)
    {
      Buttons::MIN_BUTTON.pressed = false;
      rtcManager.increaseOneMinute();
    }
    if (Buttons::START_BUTTON.pressed)
    {
      Buttons::START_BUTTON.pressed = false;
    }
  }
  else if (myDisplay.selected_line == myDisplay.pump1_line)
  {
    handle_pump_line(pump1_timer, pump1, pump2, PUMP1_ID);
  }
  else if (myDisplay.selected_line == myDisplay.pump2_line)
  {
    handle_pump_line(pump2_timer, pump2, pump1, PUMP2_ID);
  }
}

void handle_pump_line(DailyTimer &timer, PumpManager &pump, PumpManager &other_pump, int pump_id)
{
  if (Buttons::START_BUTTON.pressed)
  {
    Buttons::START_BUTTON.pressed = false;
    pump.activate_toggle(other_pump);
  }
  if (Buttons::HOUR_BUTTON.pressed)
  {
    Buttons::HOUR_BUTTON.pressed = false;
    timer.increase_one_hour();
    myDisplay.update_pump_timer(pump_id, timer);
  }
  if (Buttons::MIN_BUTTON.pressed)
  {
    Buttons::MIN_BUTTON.pressed = false;
    timer.increase_one_min();
    myDisplay.update_pump_timer(pump_id, timer);
  }
}

void timer_activation_handler(DailyTimer &p1_timer, DailyTimer &p2_timer, PumpManager &p1, PumpManager &p2)
{
  if (p1_timer.is_activation_time(p2_timer))
  {
    debugln("M-Pump1 timer - Activation start");
    p1.activate_toggle(p2);
  }

  if (p2_timer.is_activation_time(p1_timer))
  {
    debugln("M-Pump2 timer - Activation start");
    p2.activate_toggle(p1);
  }

  if (p1_timer.is_activation_timeout())
  {
    debugln("M-Pump1 timer - Activation period finished");
    p1.activate_toggle(p2);
  }

  if (p2_timer.is_activation_timeout())
  {
    debugln("M-Pump2 timer - Activation period finished");
    p2.activate_toggle(p1);
  }
}

void display_handler()
{
  if (pump1.get_activate_status() != myDisplay.pump1_last_status)
  {
    myDisplay.pump1_last_status = pump1.get_activate_status();
    if (pump1.get_activate_status())
    {
      debugln("M-Pump1 activated.");
      myDisplay.running_pump1();
    }
    else
    {
      debugln("M-Pump1 stopped.");
      myDisplay.stop_pump1();
    }
  }

  if (pump1.is_pump_tank_empty() != myDisplay.pump1_tank_empty_last_status){
    myDisplay.pump1_tank_empty_last_status = pump1.is_pump_tank_empty();
    if (myDisplay.pump1_tank_empty_last_status){
      myDisplay.empty_pump1();
    }
    else {
      if (pump1.get_activate_status()){
        myDisplay.running_pump1();
      } else {
        myDisplay.stop_pump1();
      }
    }

  }

  if (pump2.get_activate_status() != myDisplay.pump2_last_status)
  {
    myDisplay.pump2_last_status = pump2.get_activate_status();
    if (pump2.get_activate_status())
    {
      debugln("M-Pump2 activated.");
      myDisplay.running_pump2();
    }
    else
    {
      debugln("M-Pump2 stopped.");
      myDisplay.stop_pump2();
    }
  }

  if (pump2.is_pump_tank_empty() != myDisplay.pump2_tank_empty_last_status){
    myDisplay.pump2_tank_empty_last_status = pump2.is_pump_tank_empty();
    if (myDisplay.pump2_tank_empty_last_status){
      myDisplay.empty_pump2();
    } else {
      if (pump2.get_activate_status()){
        myDisplay.running_pump2();
      } else {
        myDisplay.stop_pump2();
      }
    }

  }

}

void monitor_tank_emptiness(PumpManager &pump){
  if(TankSensor::isTankEmpty()){
    pump.set_tank_empty();
    debugln("Set pump empty");
  } else {
    if (pump.is_pump_tank_empty()){
      pump.set_tank_full();
    }
  }
}