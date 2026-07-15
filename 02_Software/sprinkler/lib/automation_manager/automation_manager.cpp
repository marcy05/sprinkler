#include <automation_manager.h>

AutomationManager::AutomationManager(DailyTimer &timer1, DailyTimer &timer2,
                                     PumpManager &pump1, PumpManager &pump2)
    : timer1(timer1), timer2(timer2), pump1(pump1), pump2(pump2)
{
}

void AutomationManager::check_scheduled_activations()
{
    if (timer1.is_activation_time(timer2))
    {
        debugln("M-Pump1 timer - Activation start");
        pump1.activate_toggle(pump2);
    }

    if (timer2.is_activation_time(timer1))
    {
        debugln("M-Pump2 timer - Activation start");
        pump2.activate_toggle(pump1);
    }

    if (timer1.is_activation_timeout())
    {
        debugln("M-Pump1 timer - Activation period finished");
        pump1.activate_toggle(pump2);
    }

    if (timer2.is_activation_timeout())
    {
        debugln("M-Pump2 timer - Activation period finished");
        pump2.activate_toggle(pump1);
    }
}
