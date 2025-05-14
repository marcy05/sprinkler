#ifndef PUMPMANAGER_H
#define PUMPMANAGER_H

#include <Arduino.h>

class PumpManager{
    public:
        PumpManager(const uint8_t gpio);

        void activate_toggle();

        bool active_status = false;
    
    private:
        uint8_t gpio = 0;
};


extern PumpManager pump1;
extern PumpManager pump2;

#endif // PUMMANAGER
