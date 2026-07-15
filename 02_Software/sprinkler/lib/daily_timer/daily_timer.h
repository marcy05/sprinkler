#ifndef DAILY_TIMER_H
#define DAILY_TIMER_H

#include <RTClib.h>
#include <Preferences.h>
#include <logger.h>

class DailyTimer
{
public:
    DailyTimer(uint8_t id, RTC_DS3231 &rtc_instance);
    void begin();
    void increase_one_hour();
    void increase_one_min();
    bool is_activation_time(DailyTimer &other_timer);
    bool is_activation_timeout();

    uint8_t get_hour();
    uint8_t get_min();

    void reset_persistency();
    bool get_active_flag();

private:
    RTC_DS3231 &rtc;
    Preferences prefs;

    uint8_t user_h = 0;
    uint8_t user_m = 0;

    uint32_t start_time_sec = 0;

    uint32_t timer_activation_period = 1 * 60; // min in sec

    bool already_triggered_today = false;
    uint16_t last_checked_day = 0;

    uint8_t id = 0;

    bool m_activate_flag = false;

    void _load_state();
    void _save_state(bool triggered, uint16_t last_day);
    void _debug_loaded_state();
};

#endif // DAILY_TIMER_H