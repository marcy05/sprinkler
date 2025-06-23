#ifndef HW_ABSTRACTION_H
#define HW_ABSTRACTION_H

#include <Arduino.h>
#include <pin_config.h>
#include <logger.h>

bool setup_all_gpio();

void setup_reset_button();
void isr_reset();

void setup_deep_sleep_wakeup_button();

void setup_select_line_button();
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

void setup_pump1();
void setup_pump2();

void setup_boost_enable();

struct Button
{
    const int PIN;
    volatile bool pressed;
};

namespace Buttons
{
    extern Button RESET_BUTTON;
    extern Button DEEP_SLEEP_BUTTON;
    extern Button SELECT_LINE_BUTTON;
    extern Button HOUR_BUTTON;
    extern Button MIN_BUTTON;
    extern Button START_BUTTON;
};

namespace TankSensor{
    bool _activateTankSensor();
    bool _deactivateTankSensor();
    bool isTankEmpty();
};

#endif // HW_ABSTRACTION_H