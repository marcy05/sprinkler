#include <Arduino.h>
#include <pin_config.h>
#include <sleep_manager.h>
#include <hw_abstraction.h>
#include <sw_timer.h>
#include <logger.h>
#include <display.h>

#define DEVELOPMENT 1
#define PRODUCTION 2

#if BUILD_TYPE == PRODUCTION
  #include <nvs_flash.h>
#endif

#define SWVERSION "0.1.0"


// Declaration for SSD1306 display connected using I2C

#define SCREEN_ADDRESS 0x3C


DisplayManager myDisplay;

void setup() {
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

  switch (wakeup_reason) {
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

  //Reset
  setup_reset_button();
  debugln("Reset button setup completed.");
  
  setup_pump_switch_button();
  debugln("Switch Pump button setup completed.");
  

  if (myDisplay.begin()){
    myDisplay.main_screen();
    delay(100);
  }

  turn_on_running_led();

}

void loop() {
  // put your main code here, to run repeatedly:
  static int counter = 0;
  //debug("Cycle: ");
  //debugln(counter);


  //debug("RESET button status is: ");
  //debugln(Buttons::RESET_BUTTON.pressed);
  if (Buttons::RESET_BUTTON.pressed){
    reset_led_running_sequence();
    debugln("Reset operations");
    ESP.restart();
  }
  

  
  if (systemTimeHandler.isWakeupTimeExpired()){
    debug("System wakeup expire after: ");
    debug(systemTimeHandler.minutesWakeupPeriod);
    debugln(" minute(s).");
    debug("Entering Deep Sleep for: ");
    debug(systemTimeHandler.deepSleepWakeupAfterMinutes);
    debugln(" minute(s).");
    esp_deep_sleep_start();
  }

  if (Buttons::SWITCH_PUMP_BUTTON.pressed){
    Buttons::SWITCH_PUMP_BUTTON.pressed = false;
    myDisplay.change_pump();
    debugln("Pump pressed");
  }


  if (counter%2==0){
    debug("Current computation: ");
    debug(millis());
    debug(" - ");
    debug(systemTimeHandler.wakeupTimestamp);
    debug(" >= ");
    debugln(systemTimeHandler.minutesWakeupPeriod * 60 * 1000);
  }

  //debugln("");
  ++counter;
  delay(1000);
}

