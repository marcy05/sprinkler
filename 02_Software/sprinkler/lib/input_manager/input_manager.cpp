#include <input_manager.h>

InputManager::InputManager(DisplayManager &display, RtcManager &rtc,
                           DailyTimer &timer1, DailyTimer &timer2,
                           PumpManager &pump1, PumpManager &pump2)
    : display(display), rtc(rtc), timer1(timer1), timer2(timer2),
      pump1(pump1), pump2(pump2)
{
}

void InputManager::process_input()
{
    if (Buttons::SELECT_LINE_BUTTON.pressed)
    {
        Buttons::SELECT_LINE_BUTTON.pressed = false;
        display.change_line();
        debugln("M-Line changed");
    }

    if (display.selected_line == display.time_line)
    {
        if (Buttons::HOUR_BUTTON.pressed)
        {
            Buttons::HOUR_BUTTON.pressed = false;
            rtc.increaseOneHour();
        }
        if (Buttons::MIN_BUTTON.pressed)
        {
            Buttons::MIN_BUTTON.pressed = false;
            rtc.increaseOneMinute();
        }
        if (Buttons::START_BUTTON.pressed)
        {
            Buttons::START_BUTTON.pressed = false;
        }
    }
    else if (display.selected_line == display.pump1_line)
    {
        handle_pump_line(timer1, pump1, pump2, 1);
    }
    else if (display.selected_line == display.pump2_line)
    {
        handle_pump_line(timer2, pump2, pump1, 2);
    }
}

void InputManager::handle_pump_line(DailyTimer &timer, PumpManager &pump, PumpManager &other_pump, int pump_id)
{
    if (Buttons::START_BUTTON.pressed)
    {
        Buttons::START_BUTTON.pressed = false;
        pump.activate_toggle(other_pump);
    }
    if (Buttons::HOUR_BUTTON.pressed)
    {
        Buttons::HOUR_BUTTON.pressed = false;
        timer.increase_one_hour();
        display.update_pump_timer(pump_id, timer);
    }
    if (Buttons::MIN_BUTTON.pressed)
    {
        Buttons::MIN_BUTTON.pressed = false;
        timer.increase_one_min();
        display.update_pump_timer(pump_id, timer);
    }
}
