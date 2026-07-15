#ifndef PUMPMANAGER_H
#define PUMPMANAGER_H

#include <Arduino.h>
#include <pin_config.h>
#include <logger.h>
#include <hw_abstraction.h>

class PumpManager
{
public:
    PumpManager(const uint8_t gpio);

    void activate_toggle(PumpManager &other_pump);
    void off_pump();
    bool get_activate_status();
    bool is_pump_tank_empty();
    void set_tank_empty();
    void set_tank_full();
    void monitor_tank_status();

private:
    bool m_active_status = false;
    uint8_t m_gpio = 0;
    bool m_empty_tank = false;
};

extern PumpManager pump1;
extern PumpManager pump2;

#endif // PUMMANAGER
