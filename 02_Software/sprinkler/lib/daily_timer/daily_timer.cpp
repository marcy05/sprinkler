#include <daily_timer.h>
#include <logger.h>

DailyTimer::DailyTimer(RTC_DS3231 &rtc_instance)
    : rtc(rtc_instance), user_h(0), user_m(0),
      already_triggered_today(false), last_checked_day(0) {}

void DailyTimer::begin(){
    prefs.begin("daily-timer", false);
    prefs.end();
    _load_state();
}

void DailyTimer::increase_one_hour(){
    if (user_h >= 24){
        user_h = 0;
    } else {
       user_h = user_h + 1; 
    }
    
    prefs.begin("daily-timer", false);
    prefs.putShort("hour", user_h);
    prefs.end();
}

void DailyTimer::increase_one_min(){
    if (user_m >= 59){
        user_m = 0;
    } else {
        user_m = user_m + 1;
    }
    
    prefs.begin("daily-timer", false);
    prefs.putShort("minute", user_m);
    prefs.end();
}

bool DailyTimer::is_activation_time(){
    DateTime now = rtc.now();
    debugln();
    // If the day has changed, reset the trigger flag
    debug("Last checked day: ");
    debugln(last_checked_day);
    debug("Today: ");
    debugln(now.day());
    
    if (now.day() != last_checked_day){
        debugln("false to already_triggered_today");
        debugln("Set last_checked_day to today");
        already_triggered_today = false;
        last_checked_day = now.day();

        _save_state(already_triggered_today, last_checked_day);
    }

    DateTime target(now.year(), now.month(), now.day(), user_h, user_m, 0);

    if (!already_triggered_today && now >= target){
        already_triggered_today = true;
        debugln("Set already triggered today");

        _save_state(already_triggered_today, last_checked_day);

        start_time_sec = now.secondstime();
        debug("Set start time at: ");
        debugln(start_time_sec);

        return true;
    }

    return false;
}

bool DailyTimer::is_activation_timeout()
{
    DateTime now = rtc.now();
    if (start_time_sec != 0){
        if (now.secondstime() - start_time_sec >= timer_activation_period){
            start_time_sec = 0;
            return true;
        }
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

    debugln("Timers status:");
    debug("Altready triggered today: ");
    debugln(already_triggered_today);
    debug("Last day checked: ");
    debugln(last_checked_day);
    debug("Timer: ");
    String t = String(user_h) + ":" + String(user_m);
    debugln(t);
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


// This can be used during the development to set test time.
void DailyTimer::reset_persistency()
{
    prefs.begin("daily-timer", false);
    prefs.putBool("triggered", 0);
    prefs.putShort("last_day", 0);
    prefs.putShort("hour", 0);
    prefs.putShort("minute", 0);
    prefs.end();
}


