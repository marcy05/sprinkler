#include <pump_manager.h>

PumpManager::PumpManager(const uint8_t gpio)
{
    this->m_gpio = gpio;
    digitalWrite(this->m_gpio, LOW);
}

void PumpManager::activate_toggle()
{

    if (this->m_active_status)
    {
        digitalWrite(this->m_gpio, LOW);
        this->m_active_status = false;
    }
    else
    {
        if (TankSensor::isTankEmpty())
        {
            this->m_empty_tank = true;
            digitalWrite(this->m_gpio, LOW);
            this->m_active_status = false;
            debugln("Tank detected empty, no irrigation will start.");
        }
        else
        {
            this->m_empty_tank = false;
            digitalWrite(this->m_gpio, HIGH);
            this->m_active_status = true;
            debugln("PUM-Active status=true");
        }
    }
}

bool PumpManager::get_activate_status()
{
    return this->m_active_status;
}

PumpManager pump1 = PumpManager(Constants::PUMP1_PIN_EN);
PumpManager pump2 = PumpManager(Constants::PUMP2_PIN_EN);