#include <Arduino.h>
#include <pin_config.h>
#include <sleep_manager.h>
#include <hw_abstraction.h>
#include <sw_timer.h>
#include <logger.h>

#define DEVELOPMENT 1
#define PRODUCTION 2


void setup() {
  Serial.begin(9600);
  
  // test led setup
  setup_leds();
  debugln("Led setup completed.");

  // Timer related initialization
  systemTimeHandler.wakeupTimestamp = millis();

  // Deep sleep
  setupDeepSleep();
  setupDeepSleepWakeupAfterMins(1);
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
  
  turn_on_running_led();

}

void loop() {
  // put your main code here, to run repeatedly:
  static int counter = 0;
  debug("Cycle: ");
  debugln(counter);


  debug("RESET button status is: ");
  debugln(Buttons::RESET_BUTTON.pressed);
  if (Buttons::RESET_BUTTON.pressed){
    reset_led_running_sequence();
    debugln("Reset operations");

    Buttons::RESET_BUTTON.pressed = false;
  }
  

  
  if (systemTimeHandler.isWakeupTimeExpired()){
    debug("System wakeup expire after: ");
    debugln(systemTimeHandler.minutesWakeupPeriod);
    debugln("Entering Deep Sleep...");
    esp_deep_sleep_start();
  }



  if (counter%2==0){
    #if BUILD_TYPE == DEVELOPMENT
      debug("Current computation: ");
      debug(millis());
      debug(" - ");
      debug(systemTimeHandler.wakeupTimestamp);
      debug(" >= ");
      debugln(systemTimeHandler.minutesWakeupPeriod * 60 * 1000);
    #elif BUILD_TYPE==PRODUCTION
      debugln("Production build.");
    #endif
  }

  debugln();
  ++counter;
  delay(2000);
}

