#include <pump_manager.h>

PumpManager::PumpManager(const uint8_t gpio)
{
    this->m_gpio = gpio;
    digitalWrite(this->m_gpio, LOW);
}

void PumpManager::activate_toggle(PumpManager &other_pump)
{
    if(other_pump.get_activate_status()){
        other_pump.off_pump();
    }

    if (this->m_active_status)
    {
        PumpManager::off_pump();
    }
    else
    {
        digitalWrite(this->m_gpio, HIGH);
        this->m_active_status = true;
    }
}

void PumpManager::off_pump(){
    digitalWrite(this->m_gpio, LOW);
    this->m_active_status = false;
}

bool PumpManager::get_activate_status()
{
    return this->m_active_status;
}

bool PumpManager::is_pump_tank_empty()
{
    return this->m_empty_tank;
}

void PumpManager::set_tank_empty(){
    this->m_empty_tank = true;
    this->m_active_status = false;
    digitalWrite(this->m_gpio, LOW);
    debugln("The tank is empty, deactivate all.");
}

void PumpManager::set_tank_full(){
    this->m_empty_tank = false;
    debugln("The tank is full, ready for activation.");
}

PumpManager pump1 = PumpManager(Constants::PUMP1_PIN_EN);
PumpManager pump2 = PumpManager(Constants::PUMP2_PIN_EN);