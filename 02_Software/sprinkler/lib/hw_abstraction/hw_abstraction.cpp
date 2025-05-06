#include <hw_abstraction.h>
#include <Arduino.h>
#include <pin_config.h>


namespace Buttons {
    Button RESET_BUTTON = {Constants::RESET_BUTTON_PIN, false};
    Button DEEP_SLEEP_BUTTON = {Constants::DEEP_SLEEP, false};
}

void setup_leds()
{
    pinMode(Constants::LED_RUNNING, OUTPUT);
    pinMode(Constants::LED_BATTERY, OUTPUT);

    digitalWrite(Constants::LED_RUNNING, HIGH);
    digitalWrite(Constants::LED_BATTERY, HIGH);
}

void turn_on_running_led()
{
    digitalWrite(Constants::LED_RUNNING, LOW);
}

void turn_off_running_led()
{
    digitalWrite(Constants::LED_RUNNING, HIGH);
}

void reset_led_running_sequence()
{
    const uint8_t iterations = 3;
    for (u_int8_t i=0; i<iterations; i++){
        turn_off_running_led();
        delay(300);
        turn_on_running_led();
        delay(300);
    }
}

void wakeup_from_sleep_sequence()
{
    const uint8_t iterations = 2;
    for (u_int8_t i=0; i<iterations; i++){
        turn_off_running_led();
        delay(100);
        turn_on_running_led();
        delay(100);
    }
}

void turn_on_battery_led()
{
    digitalWrite(Constants::LED_BATTERY, LOW);
}

void turn_off_battery_led()
{
    digitalWrite(Constants::LED_BATTERY, HIGH);
}

void setup_reset_button()
{
    pinMode(Buttons::RESET_BUTTON.PIN, INPUT_PULLUP);
    attachInterrupt(Buttons::RESET_BUTTON.PIN, isr, FALLING);
}

void IRAM_ATTR isr()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
  
    if (interrupt_time - last_interrupt_time > 1000) {
      if (!Buttons::RESET_BUTTON.pressed){
        Buttons::RESET_BUTTON.pressed = true;
      } else {
        Buttons::RESET_BUTTON.pressed = false;
      }
      
      last_interrupt_time = interrupt_time;
    }
}



