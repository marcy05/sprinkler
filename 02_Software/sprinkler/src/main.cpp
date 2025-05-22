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

// Declaration for SSD1306 display connected using I2C

#define SCREEN_ADDRESS 0x3C

DisplayManager myDisplay;
RTC_DS3231 rtc;
RtcManager rtcManager(rtc);

DailyTimer pump1_timer(rtc);
DailyTimer pump2_timer(rtc);

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

  // test led setup
  setup_leds();
  debugln("Led setup completed.");

  // Timer related initialization
  systemTimeHandler.wakeupTimestamp = millis();

  // Deep sleep
  setupDeepSleep();

#if BUILD_TYPE == DEVELOPMENT
  systemTimeHandler.deepSleepWakeupAfterMinutes = 1;
#endif

  setupDeepSleepWakeupAfterMins(systemTimeHandler.deepSleepWakeupAfterMinutes);
  debugln("Deep spleep setup completed.");

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1:
    wakeup_from_sleep_sequence();
    Buttons::DEEP_SLEEP_BUTTON.pressed = true;
    debugln("Wakeup due to button");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    wakeup_from_sleep_sequence();
    debugln("Wakeup caused by timer");
    break;
  default:
    debugln("Standard wakeup");
  }

  // Reset
  setup_reset_button();
  debugln("Reset button setup completed.");

  setup_pump_switch_button();
  debugln("Switch Pump button setup completed.");

  setup_hour_button();
  setup_min_button();
  debugln("Setup Hour and Min buttons.");

  setup_start_button();
  debugln("Setup start button");

  Wire.begin();

  rtc.begin();
  debugln("Setup RTC");

  if (myDisplay.begin())
  {

    myDisplay.main_screen(rtc.now());
    delay(100);
  }

  // TODO remove if after tests
  pump1_timer.reset_persistency();
  pump2_timer.reset_persistency();
  
  pump1_timer.begin();
  pump2_timer.begin();

  debugln("Setup pump Timers");

  myDisplay.update_pump_timer(1, pump1_timer);
  myDisplay.update_pump_timer(2, pump2_timer);

  turn_on_running_led();

  setup_water_sensor_enable();
  setup_water_sensor_signal_input();
  setup_display_enable();
}

void loop()
{
  // put your main code here, to run repeatedly:
  static int counter = 0;
  // debug("Cycle: ");
  // debugln(counter);

  DateTime now = rtc.now();
  myDisplay.update_time(now);
  //myDisplay.update_pump_timer(1, pump1_timer);
  //myDisplay.update_pump_timer(2, pump2_timer);

  debug("Hour: ");
  debug(now.hour());
  debug(" Min: ");
  debugln(now.minute());

  // debug("RESET button status is: ");
  // debugln(Buttons::RESET_BUTTON.pressed);
  if (Buttons::RESET_BUTTON.pressed)
  {
    reset_led_running_sequence();
    debugln("Reset operations");
    ESP.restart();
  }

  if (systemTimeHandler.isWakeupTimeExpired())
  {
    debug("System wakeup expire after: ");
    debug(systemTimeHandler.minutesWakeupPeriod);
    debugln(" minute(s).");
    debug("Entering Deep Sleep for: ");
    debug(systemTimeHandler.deepSleepWakeupAfterMinutes);
    debugln(" minute(s).");
    myDisplay.sleep_screen();
    esp_deep_sleep_start();
  }

  if (Buttons::SELECT_LINE_BUTTON.pressed)
  {
    Buttons::SELECT_LINE_BUTTON.pressed = false;
    myDisplay.change_line();
    debugln("Line changed");
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
      debugln("Change START for pump 1");
      myDisplay.running_pump1();
    }
    else
    {
      debugln("Change STOP for pump 1");
      myDisplay.stop_pump1();
    }
  }

  if (pump2.get_activate_status() != myDisplay.pump2_last_status)
  {
    myDisplay.pump2_last_status = pump2.get_activate_status();
    if (pump2.get_activate_status())
    {
      debugln("Change START for pump 2");
      myDisplay.running_pump2();
    }
    else
    {
      debugln("Change STOP for pump 1");
      myDisplay.stop_pump2();
    }
  }

  if (pump1_timer.is_activation_time()){
    debugln("Pump1 timer - Activation start");
    pump1.activate_toggle();
    debug("Display act status1: ");
    debugln(myDisplay.pump1_last_status);
  }

  if (pump2_timer.is_activation_time()){
    debugln("Pump2 timer - Activation start");
    pump2.activate_toggle();
    debug("Display act status2: ");
    debugln(myDisplay.pump2_last_status);
  }

  if (pump1_timer.is_activation_timeout()){
    debugln("Pump1 timer - Activation period finished");
    pump1.activate_toggle();
  }

  if (pump2_timer.is_activation_timeout()){
    debugln("Pump2 timer - Activation period finished");
    pump2.activate_toggle();
  }

  /*
  if (counter % 2 == 0)
  {
    debug("Current computation: ");
    debug(millis());
    debug(" - ");
    debug(systemTimeHandler.wakeupTimestamp);
    debug(" >= ");
    debugln(systemTimeHandler.minutesWakeupPeriod * 60 * 1000);
    debug("Water sensor read: ");
    debugln(digitalRead(Constants::WATER_SENSOR_SIG));
  }*/

  // debugln("");
  ++counter;
  delay(500);
}
