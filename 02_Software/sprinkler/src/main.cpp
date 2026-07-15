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
#include <input_manager.h>
#include <automation_manager.h>
#include <Wire.h>

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

InputManager inputManager(myDisplay, rtcManager, pump1_timer, pump2_timer, pump1, pump2);
AutomationManager automationManager(pump1_timer, pump2_timer, pump1, pump2);

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

  inputManager.process_input();
  automationManager.check_scheduled_activations();
  myDisplay.update_display_state(pump1, pump2);
  pump1.monitor_tank_status();
  pump2.monitor_tank_status();

  // debug("Water sensor. Digital: ");
  // int value = digitalRead(Constants::WATER_SENSOR_SIG);
  // debug(value);
  // debug(" TankEmpty?: ");
  // debugln(TankSensor::isTankEmpty());

  debug("Pump1 empty? ");
  debugln(pump1.is_pump_tank_empty());
  debug("Pump2 empty? ");
  debugln(pump2.is_pump_tank_empty());

  delay(100);
}