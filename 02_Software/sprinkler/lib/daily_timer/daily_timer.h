#ifndef DAILY_TIMER_H
#define DAILY_TIMER_H

#include <RTClib.h>
#include <Preferences.h>

class DailyTimer{
    public:
        DailyTimer(RTC_DS3231 &rtc_instance);
        void begin();
        void set_target_time(uint8_t hour, uint8_t minute);
        bool is_time_expired();
        //void print_now();
    
    private:
        RTC_DS3231 &rtc;
        Preferences prefs;

        uint8_t user_h = 0;
        uint8_t user_m = 0;
        
        bool already_triggered_today = false;
        uint16_t last_checked_day = 0;

        void _load_state();
        void _save_state(bool triggered, uint16_t last_day);
};




#endif // DAILY_TIMER_H