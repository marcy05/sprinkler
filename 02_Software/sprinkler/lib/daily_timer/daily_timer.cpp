#include <daily_timer.h>

DailyTimer::DailyTimer(RTC_DS3231 &rtc_instance)
    : rtc(rtc_instance), user_h(0), user_m(0),
      already_triggered_today(false), last_checked_day(0) {}

void DailyTimer::begin(){
    prefs.begin("daily-timer", false);
    _load_state();
    prefs.end();
}

void DailyTimer::increase_one_hour(){
    user_h = user_h + 1;
    prefs.begin("daily-timer", false);
    prefs.putShort("hour", user_h);
    prefs.end();
}

void DailyTimer::increase_one_min(){
    user_m = user_m + 1;
    prefs.begin("daily-timer", false);
    prefs.putShort("minute", user_m);
    prefs.end();
}

bool DailyTimer::is_time_expired(){
    DateTime now = rtc.now();

    // If the day has changed, reset the trigger flag
    if (now.day() != last_checked_day){
        already_triggered_today = false;
        last_checked_day = now.day();

        prefs.begin("daily-timer", false);
        _save_state(already_triggered_today, last_checked_day);
        prefs.end();
    }

    DateTime target(now.year(), now.month(), now.day(), user_h, user_m, 0);

    if (!already_triggered_today && now >= target){
        already_triggered_today = true;

        prefs.begin("daily-timer", false);
        _save_state(already_triggered_today, last_checked_day);
        prefs.end();

        return true;
    }

    return false;
}

void DailyTimer::_load_state(){
    prefs.begin("daily-timer", true);
    already_triggered_today = prefs.getBool("triggered", false);
    last_checked_day = prefs.getShort("last_day", 0);
    user_h = prefs.getShort("hour", 0);
    user_m = prefs.getShort("minute", 0);
    prefs.end();
}

void DailyTimer::_save_state(bool triggered, uint16_t last_day){
    prefs.begin("daily-timer", false);
    prefs.putBool("triggered", triggered);
    prefs.putShort("last_day", last_day);
    prefs.end();
}

uint8_t DailyTimer::get_hour(){
    return user_h;
}

uint8_t DailyTimer::get_min(){
    return user_m;
}
// void DailyTimer::print_now(){
//     DateTime now = rtc.now();
// }


