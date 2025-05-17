#include <hw_abstraction.h>
#include <Arduino.h>
#include <pin_config.h>
//#include <sync_utils.h>

//portMUX_TYPE buttonMux = portMUX_INITIALIZER_UNLOCKED;


namespace Buttons {
  Button RESET_BUTTON = {Constants::RESET_BUTTON_PIN, false};
  Button DEEP_SLEEP_BUTTON = {Constants::DEEP_SLEEP_WAKEUP_BUTTON_PIN, false};
  Button SELECT_LINE_BUTTON = {Constants::SELECT_LINE_BUTTON_PIN, false};
  Button HOUR_BUTTON = {Constants::HOUR_BUTTON_PIN, false};
  Button MIN_BUTTON = {Constants::MIN_BUTTON_PIN, false};
  Button START_BUTTON = {Constants::START_BUTTON_PIN, false};
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
  attachInterrupt(Buttons::RESET_BUTTON.PIN, isr_reset, FALLING);
}

void IRAM_ATTR isr_reset()
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

void setup_pump_switch_button()
{
  pinMode(Buttons::SELECT_LINE_BUTTON.PIN, INPUT_PULLUP);
  attachInterrupt(Buttons::SELECT_LINE_BUTTON.PIN, isr_switch_pump, FALLING);
}

void IRAM_ATTR isr_switch_pump()
{
  Buttons::SELECT_LINE_BUTTON.pressed = true;
}


void setup_hour_button()
{
  pinMode(Buttons::HOUR_BUTTON.PIN, INPUT_PULLUP);
  attachInterrupt(Buttons::HOUR_BUTTON.PIN, isr_hour, FALLING);
}

void IRAM_ATTR isr_hour()
{
  Buttons::HOUR_BUTTON.pressed = true;
}


void setup_min_button()
{
  pinMode(Buttons::MIN_BUTTON.PIN, INPUT_PULLUP);
  attachInterrupt(Buttons::MIN_BUTTON.PIN, isr_min, FALLING);
}

void IRAM_ATTR isr_min()
{
  Buttons::MIN_BUTTON.pressed = true;
}


void setup_start_button()
{
  pinMode(Buttons::START_BUTTON.PIN, INPUT_PULLUP);
  attachInterrupt(Buttons::START_BUTTON.PIN, isr_start, FALLING);
}

void IRAM_ATTR isr_start()
{
  Buttons::START_BUTTON.pressed = true;
}

void setup_water_sensor_enable()
{
  pinMode(Constants::WATER_SENSOR_EN, OUTPUT);
}

void setup_water_sensor_signal_input()
{
  pinMode(Constants::WATER_SENSOR_SIG, INPUT_PULLUP);
}

void setup_display_enable()
{
  pinMode(Constants::DISPLAY_EN, OUTPUT);
}
