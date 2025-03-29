#include <Arduino.h>
#include <pin_config.h>
#include <sleep_manager.h>
#include <hw_abstraction.h>
#include <sw_timer.h>

#define DEVELOPMENT 1
#define PRODUCTION 2

void setup() {
  Serial.begin(9600);
  
  // test led setup
  setup_leds();
  Serial.println("Led setup completed.");

  // Timer related initialization
  systemTimeHandler.wakeupTimestamp = millis();

  // Deep sleep
  setupDeepSleep();
  setupDeepSleepWakeupAfterMins(1);
  Serial.println("Deep spleep setup completed.");

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT1:
      wakeup_from_sleep_sequence();
      Buttons::DEEP_SLEEP_BUTTON.pressed = true;
      Serial.println("Wakeup due to button");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      wakeup_from_sleep_sequence();
      Serial.println("Wakeup caused by timer");
      break;
    default:
      Serial.println("Standard wakeup");
  }

  
  //Reset
  setup_reset_button();
  Serial.println("Reset button setup completed.");
  
  turn_on_running_led();

}

void loop() {
  // put your main code here, to run repeatedly:
  static int counter = 0;
  Serial.print("Cycle: ");
  Serial.println(counter);


  Serial.print("RESET button status is: ");
  Serial.println(Buttons::RESET_BUTTON.pressed);
  if (Buttons::RESET_BUTTON.pressed){
    reset_led_running_sequence();
    Serial.println("Reset operations");

    Buttons::RESET_BUTTON.pressed = false;
  }
  

  
  if (systemTimeHandler.isWakeupTimeExpired()){
    Serial.print("System wakeup expire after: ");
    Serial.println(systemTimeHandler.minutesWakeupPeriod);
    Serial.println("Entering Deep Sleep...");
    esp_deep_sleep_start();
  }



  if (counter%2==0){
    #if BUILD_TYPE == DEVELOPMENT
      Serial.print("Current computation: ");
      Serial.print(millis());
      Serial.print(" - ");
      Serial.print(systemTimeHandler.wakeupTimestamp);
      Serial.print(" >= ");
      Serial.println(systemTimeHandler.minutesWakeupPeriod * 60 * 1000);
    #elif BUILD_TYPE==PRODUCTION
      Serial.println("Production build.");
    #endif
  }

  Serial.println();
  ++counter;
  delay(2000);
}

