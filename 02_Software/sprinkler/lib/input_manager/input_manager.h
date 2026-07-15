#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <display.h>
#include <rtc_manager.h>
#include <daily_timer.h>
#include <pump_manager.h>
#include <hw_abstraction.h>
#include <logger.h>

class InputManager
{
public:
    InputManager(DisplayManager &display, RtcManager &rtc,
                 DailyTimer &timer1, DailyTimer &timer2,
                 PumpManager &pump1, PumpManager &pump2);

    void process_input();

private:
    DisplayManager &display;
    RtcManager &rtc;
    DailyTimer &timer1;
    DailyTimer &timer2;
    PumpManager &pump1;
    PumpManager &pump2;

    void handle_pump_line(DailyTimer &timer, PumpManager &pump, PumpManager &other_pump, int pump_id);
};

#endif // INPUT_MANAGER_H
