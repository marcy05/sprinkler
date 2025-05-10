#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H


namespace Constants {
    constexpr uint8_t DEEP_SLEEP_WAKEUP_BUTTON = 32; // It requires a pull-up resistor

    constexpr uint8_t LED_RUNNING = 27;
    constexpr uint8_t LED_BATTERY = 26;

    constexpr uint8_t RESET_BUTTON_PIN = 33;

    constexpr uint8_t SWITCH_PUMP_PIN = 25;
}






#endif // PIN_CONFIG_H