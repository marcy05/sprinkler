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

DailyTimer pump1_timer(1, rtc);
DailyTimer pump2_timer(2, rtc);


/******************************************************************************
                                    FUNCTIONS
******************************************************************************/



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


  // Timer related initialization
  systemTimeHandler.wakeupTimestamp = millis();

  // We are going to run, deactivate all alarms to avoid unexpected shutdown.
  rtcManager.deactivateAlarm();

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

  // Reset
  setup_reset_button();
  debugln("M-Reset button setup completed.");

  setup_pump_switch_button();
  debugln("M-Switch Pump button setup completed.");

  setup_hour_button();
  setup_min_button();
  debugln("M-Setup Hour and Min buttons.");

  setup_start_button();
  debugln("M-Setup start button");

  Wire.begin();
  debugln("M-I2C setup done.");

  rtc.begin();
  debugln("M-Setup RTC done.");

  if (myDisplay.begin())
  {

    myDisplay.main_screen(rtc.now());
    delay(100);
  }

  #if BUILD_TYPE == DEVELOPMENT
    pump1_timer.reset_persistency();
    pump2_timer.reset_persistency();
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

void loop()
{

  DateTime now = rtc.now();
  myDisplay.update_time(now);
  //myDisplay.update_pump_timer(1, pump1_timer);
  //myDisplay.update_pump_timer(2, pump2_timer);
  debugln();
  debugln(now.timestamp());

  // debug("RESET button status is: ");
  // debugln(Buttons::RESET_BUTTON.pressed);
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
    // esp_deep_sleep_start();
    rtcManager.activateAlarm();
  }

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
    if (Buttons::START_BUTTON.pressed){
      Buttons::START_BUTTON.pressed = false;
    }
  }
  else if (myDisplay.selected_line == myDisplay.pump1_line)
  {
    if (Buttons::START_BUTTON.pressed)
    {
      Buttons::START_BUTTON.pressed = false;
      pump1.activate_toggle();
    }
    if (Buttons::HOUR_BUTTON.pressed){
      Buttons::HOUR_BUTTON.pressed = false;
      pump1_timer.increase_one_hour();
      myDisplay.update_pump_timer(1, pump1_timer);
    }
    if (Buttons::MIN_BUTTON.pressed){
      Buttons::MIN_BUTTON.pressed = false;
      pump1_timer.increase_one_min();
      myDisplay.update_pump_timer(1, pump1_timer);
    }
  }
  else if (myDisplay.selected_line == myDisplay.pump2_line)
  {
    if (Buttons::START_BUTTON.pressed)
    {
      Buttons::START_BUTTON.pressed = false;
      pump2.activate_toggle();
    }
    if (Buttons::HOUR_BUTTON.pressed){
      Buttons::HOUR_BUTTON.pressed = false;
      pump2_timer.increase_one_hour();
      myDisplay.update_pump_timer(2, pump2_timer);
    }
    if (Buttons::MIN_BUTTON.pressed){
      Buttons::MIN_BUTTON.pressed = false;
      pump2_timer.increase_one_min();
      myDisplay.update_pump_timer(2, pump2_timer);
    }
  }

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

  if (pump1_timer.is_activation_time()){
    debugln("M-Pump1 timer - Activation start");
    pump1.activate_toggle();
  }

  if (pump2_timer.is_activation_time()){
    debugln("M-Pump2 timer - Activation start");
    pump2.activate_toggle();

  }

  if (pump1_timer.is_activation_timeout()){
    debugln("M-Pump1 timer - Activation period finished");
    pump1.activate_toggle();
  }

  if (pump2_timer.is_activation_timeout()){
    debugln("M-Pump2 timer - Activation period finished");
    pump2.activate_toggle();
  }



  delay(100);
}
