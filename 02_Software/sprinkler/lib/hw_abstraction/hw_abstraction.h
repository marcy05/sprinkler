#ifndef HW_ABSTRACTION_H
#define HW_ABSTRACTION_H


void setup_leds();

void turn_on_running_led();
void turn_off_running_led();

void reset_led_running_sequence();
void wakeup_from_sleep_sequence();

void turn_on_battery_led();
void turn_off_battery_led();


void setup_reset_button();
void isr_reset();

void setup_pump_switch_button();
void isr_switch_pump();

void setup_hour_button();
void isr_hour();

void setup_min_button();
void isr_min();

void setup_start_button();
void isr_start();

void setup_water_sensor_enable();
void setup_water_sensor_signal_input();

void setup_display_enable();

struct Button
{
    const int PIN;
    volatile bool pressed;
};

namespace Buttons {
    extern Button RESET_BUTTON;
    extern Button DEEP_SLEEP_BUTTON;
    extern Button SELECT_LINE_BUTTON;
    extern Button HOUR_BUTTON;
    extern Button MIN_BUTTON;
    extern Button START_BUTTON;
};


#endif // HW_ABSTRACTION_H