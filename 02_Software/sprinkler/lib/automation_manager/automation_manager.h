#ifndef AUTOMATION_MANAGER_H
#define AUTOMATION_MANAGER_H

#include <Arduino.h>
#include <daily_timer.h>
#include <pump_manager.h>
#include <logger.h>

class AutomationManager
{
public:
    AutomationManager(DailyTimer &timer1, DailyTimer &timer2,
                      PumpManager &pump1, PumpManager &pump2);

    void check_scheduled_activations();

private:
    DailyTimer &timer1;
    DailyTimer &timer2;
    PumpManager &pump1;
    PumpManager &pump2;
};

#endif // AUTOMATION_MANAGER_H
