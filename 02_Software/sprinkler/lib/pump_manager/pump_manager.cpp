#include <pump_manager.h>

PumpManager::PumpManager(const uint8_t gpio)
{
    this->gpio = gpio;
    pinMode(this->gpio, OUTPUT);
    digitalWrite(this->gpio, LOW);
}

void PumpManager::activate_toggle()
{

    if (this->active_status)
    {
        digitalWrite(this->gpio, LOW);
        this->active_status = false;
    }
    else
    {
        digitalWrite(this->gpio, HIGH);
        this->active_status = true;
        debugln("PUM-Active status=true");
    }
}

bool PumpManager::get_activate_status()
{
    return this->active_status;
}

PumpManager pump1 = PumpManager(Constants::PUMP1_PIN_EN);
PumpManager pump2 = PumpManager(Constants::PUMP2_PIN_EN);