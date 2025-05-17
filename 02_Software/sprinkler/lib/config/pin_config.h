#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H


namespace Constants {
    constexpr uint8_t WATER_SENSOR_EN = 0;
    constexpr uint8_t WATER_SENSOR_SIG = 4;
    constexpr uint8_t DISPLAY_EN = 2;

    constexpr uint8_t START_BUTTON_PIN = 14;
    constexpr uint8_t PUMP1_PIN_EN = 19;
    constexpr uint8_t PUMP2_PIN_EN = 18;

    constexpr uint8_t SELECT_LINE_BUTTON_PIN = 25;
    constexpr uint8_t MIN_BUTTON_PIN = 26;
    constexpr uint8_t HOUR_BUTTON_PIN = 27;

    constexpr uint8_t DEEP_SLEEP_WAKEUP_BUTTON_PIN = 32; // It requires a pull-up resistor
    constexpr uint8_t RESET_BUTTON_PIN = 33;

    constexpr uint8_t LED_BATTERY = 12;
    constexpr uint8_t LED_RUNNING = 13;

}






#endif // PIN_CONFIG_H