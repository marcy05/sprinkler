#include <daily_timer.h>

DailyTimer::DailyTimer(uint8_t id, RTC_DS3231 &rtc_instance)
    : rtc(rtc_instance), user_h(0), user_m(0),
      already_triggered_today(false), last_checked_day(0), id(id) {}

void DailyTimer::begin()
{
    prefs.begin("daily-timer", false);
    prefs.end();
    _load_state();
}

void DailyTimer::increase_one_hour()
{
    if (user_h >= 24)
    {
        user_h = 0;
    }
    else
    {
        user_h = user_h + 1;
    }

    prefs.begin("daily-timer", false);
    prefs.putShort("hour", user_h);
    prefs.end();

    debug("DAT-PumpId: ");
    debugln(id);
    debugln("DAT-Increased one our.");
}

void DailyTimer::increase_one_min()
{
    if (user_m >= 59)
    {
        user_m = 0;
    }
    else
    {
        user_m = user_m + 1;
    }

    prefs.begin("daily-timer", false);
    prefs.putShort("minute", user_m);
    prefs.end();
    debugln("DAT-Increased one minute.");
}

bool DailyTimer::is_activation_time()
{
    DateTime now = rtc.now();
    // If the day has changed, reset the trigger flag
    // debug("DAT-Last checked day: ");
    // debugln(last_checked_day);
    // debug("DAT-Today: ");
    // debugln(now.day());

    if (now.day() != last_checked_day)
    {
        debug("DAT-PumpId: ");
        debugln(id);
        debugln("DAT-Reset already_triggered_today, new day.");
        already_triggered_today = false;
        last_checked_day = now.day();

        _save_state(already_triggered_today, last_checked_day);
    }

    DateTime target(now.year(), now.month(), now.day(), user_h, user_m, 0);

    if (!already_triggered_today && now >= target)
    {
        already_triggered_today = true;
        debug("DAT-PumpId: ");
        debugln(id);
        debugln("DAT-Set already_triggered_today to true");

        _save_state(already_triggered_today, last_checked_day);

        start_time_sec = now.secondstime();
        debug("DAT-Set start time at: ");
        debugln(start_time_sec);

        return true;
    }

    return false;
}

bool DailyTimer::is_activation_timeout()
{
    DateTime now = rtc.now();
    if (start_time_sec != 0)
    {
        debug("DAT-PumpId: ");
        debugln(id);
        debug("DAT-Time before activation stops: ");
        debugln(timer_activation_period + start_time_sec - now.secondstime());
        if (now.secondstime() - start_time_sec >= timer_activation_period)
        {
            start_time_sec = 0;
            return true;
        }
    }

    return false;
}

void DailyTimer::_load_state()
{
    prefs.begin("daily-timer", true);
    already_triggered_today = prefs.getBool("triggered", false);
    last_checked_day = prefs.getShort("last_day", 0);
    user_h = prefs.getShort("hour", 0);
    user_m = prefs.getShort("minute", 0);
    prefs.end();

    _debug_loaded_state();
}

void DailyTimer::_save_state(bool triggered, uint16_t last_day)
{
    prefs.begin("daily-timer", false);
    prefs.putBool("triggered", triggered);
    prefs.putShort("last_day", last_day);
    prefs.end();
}

void DailyTimer::_debug_loaded_state()
{
    debug("DAT-PumpId: ");
    debugln(id);
    debugln("DAT-debug timer loaded variables:");
    debugln("DAT-Timers status:");
    debug("DAT-Altready triggered today: ");
    debugln(already_triggered_today);
    debug("DAT-Last day checked: ");
    debugln(last_checked_day);
    debug("DAT-Timer: ");
    String t = String(user_h) + ":" + String(user_m);
    debugln(t);
}

uint8_t DailyTimer::get_hour()
{
    return user_h;
}

uint8_t DailyTimer::get_min()
{
    return user_m;
}

// This can be used during the development to set test time.
void DailyTimer::reset_persistency()
{
    debug("DAT-PumpId: ");
    debugln(id);
    debugln("DAT-Reset persistency.");
    prefs.begin("daily-timer", false);
    prefs.putBool("triggered", 0);
    prefs.putShort("last_day", 0);
    prefs.putShort("hour", 0);
    prefs.putShort("minute", 0);
    prefs.end();
}
