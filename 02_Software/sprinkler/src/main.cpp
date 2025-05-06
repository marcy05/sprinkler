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
  


  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  

  turn_on_running_led();

  // Clear the buffer.
  display.clearDisplay();

  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display Inverted Text
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,28);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Changing Font Size
  display.setTextColor(WHITE);
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println("Hello!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display Numbers
  display.setTextSize(1);
  display.setCursor(0,28);
  display.println(123456789);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Specifying Base For Numbers
  display.setCursor(0,28);
  display.print("0x"); display.print(0xFF, HEX); 
  display.print("(HEX) = ");
  display.print(0xFF, DEC);
  display.println("(DEC)"); 
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display ASCII Characters
  display.setCursor(0,24);
  display.setTextSize(2);
  display.write(3);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Scroll full screen
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Full");
  display.println("screen");
  display.println("scrolling!");
  display.display();
  display.startscrollright(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  display.clearDisplay();

  // Scroll part of the screen
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Scroll");
  display.println("some part");
  display.println("of the screen.");
  display.display();
  display.startscrollright(0x00, 0x00);

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
    reset_led_running_sequence();
    Buttons::RESET_BUTTON.pressed = false;
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
  delay(2000);
}

