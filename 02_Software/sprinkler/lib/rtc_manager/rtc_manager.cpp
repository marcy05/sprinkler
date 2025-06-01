#include <rtc_manager.h>

RtcManager::RtcManager(RTC_DS3231 &rtcRef) : rtc(rtcRef)
{
}

void RtcManager::increaseOneHour()
{
    DateTime now = rtc.now();
    int newHour = (now.hour() + 1) % 24;
    DateTime newTime(now.year(), now.month(), now.day(), newHour, now.minute(), now.second());
    rtc.adjust(newTime);
    debug("RTC-New time set: ");
    debugln(rtc.now().timestamp(DateTime::TIMESTAMP_TIME));
}

void RtcManager::increaseOneMinute()
{
    DateTime now = rtc.now();
    int newHMin = (now.minute() + 1) % 60;
    DateTime newTime(now.year(), now.month(), now.day(), now.hour(), newHMin, now.second());

    rtc.adjust(newTime);
    debug("RTC-New time set: ");
    debugln(rtc.now().timestamp(DateTime::TIMESTAMP_TIME));
}

void RtcManager::activateAlarm()
{
    uint8_t activation_period_min = 15; // activate every n minute

    DateTime now = rtc.now();

    DateTime alarm1 = now + TimeSpan(activation_period_min * 60);

    if (!rtc.setAlarm1(alarm1, DS3231_A1_Minute))
    {
        debugln("SLM-Error, alarm wan't set");
    }
    else
    {
        debug("Alarm will be triggered at: ");
        debugln(alarm1.timestamp());
    }
}

void RtcManager::deactivateAlarm()
{
    rtc.clearAlarm(1);
    rtc.clearAlarm(2); // Even if not used.
    rtc.disableAlarm(2);

    rtc.writeSqwPinMode(DS3231_OFF);
}
