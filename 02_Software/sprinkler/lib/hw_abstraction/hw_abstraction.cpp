#include <hw_abstraction.h>

namespace Buttons
{
  Button RESET_BUTTON = {Constants::RESET_BUTTON_PIN, false};
  Button DEEP_SLEEP_BUTTON = {Constants::DEEP_SLEEP_WAKEUP_BUTTON_PIN, false};
  Button SELECT_LINE_BUTTON = {Constants::SELECT_LINE_BUTTON_PIN, false};
  Button HOUR_BUTTON = {Constants::HOUR_BUTTON_PIN, false};
  Button MIN_BUTTON = {Constants::MIN_BUTTON_PIN, false};
  Button START_BUTTON = {Constants::START_BUTTON_PIN, false};
}

bool setup_all_gpio(){
  setup_reset_button();
  setup_deep_sleep_wakeup_button();
  setup_select_line_button();
  setup_hour_button();
  setup_min_button();
  setup_start_button();
  setup_water_sensor_enable();
  setup_water_sensor_signal_input();
  setup_display_enable();

  setup_pump1();
  setup_pump2();

  void setup_boost_enable();

  debugln("All used GPIO setup completed.");
  return true;
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

  if (interrupt_time - last_interrupt_time > 1000)
  {
    if (!Buttons::RESET_BUTTON.pressed)
    {
      Buttons::RESET_BUTTON.pressed = true;
    }
    else
    {
      Buttons::RESET_BUTTON.pressed = false;
    }

    last_interrupt_time = interrupt_time;
  }
}

void setup_deep_sleep_wakeup_button(){
  pinMode(Buttons::DEEP_SLEEP_BUTTON.PIN, INPUT_PULLUP);
}

void setup_select_line_button()
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
  debugln("HWA-Setup water sensor enable GPIO.");
}

void setup_water_sensor_signal_input()
{
  pinMode(Constants::WATER_SENSOR_SIG, INPUT_PULLUP);
  debugln("HWA-Setup water sensor signal GPIO.");
}

void setup_display_enable()
{
  pinMode(Constants::DISPLAY_EN, OUTPUT);
  debugln("HWA-Setup display enable GPIO.");
}

void setup_pump1(){
  pinMode(Constants::PUMP1_PIN_EN, OUTPUT);
}

void setup_pump2(){
  pinMode(Constants::PUMP2_PIN_EN, OUTPUT);
}

void setup_boost_enable(){
  pinMode(Constants::BOOST_EN, OUTPUT);
}