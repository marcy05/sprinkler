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
void isr();

struct Button
{
    const int PIN;
    volatile bool pressed;
};

namespace Buttons {
    extern Button RESET_BUTTON;
    extern Button DEEP_SLEEP_BUTTON;
}


#endif // HW_ABSTRACTION_H