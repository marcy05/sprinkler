#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

namespace Constants
{
    constexpr uint8_t WATER_SENSOR_EN = 23;
    constexpr uint8_t WATER_SENSOR_SIG = 19;
    constexpr uint8_t DISPLAY_EN = 32;

    constexpr uint8_t START_BUTTON_PIN = 34;
    constexpr uint8_t PUMP1_PIN_EN = 27;
    constexpr uint8_t PUMP2_PIN_EN = 26;

    constexpr uint8_t SELECT_LINE_BUTTON_PIN = 35;
    constexpr uint8_t MIN_BUTTON_PIN = 17;
    constexpr uint8_t HOUR_BUTTON_PIN = 5;

    constexpr uint8_t DEEP_SLEEP_WAKEUP_BUTTON_PIN = 4;
    constexpr uint8_t ALARM_SQW_PIN = DEEP_SLEEP_WAKEUP_BUTTON_PIN;
    constexpr uint8_t RESET_BUTTON_PIN = 18;

    constexpr uint8_t BOOST_EN = 2;

    constexpr uint8_t BATTERY_CHARGE_EN = 25;

}

#endif // PIN_CONFIG_H