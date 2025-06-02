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

    void activate_toggle();

    bool get_activate_status();

private:
    bool active_status = false;
    uint8_t gpio = 0;
    bool empty_tank = false;

    bool _is_tank_empty();
};

extern PumpManager pump1;
extern PumpManager pump2;

#endif // PUMMANAGER
