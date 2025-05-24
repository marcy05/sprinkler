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
