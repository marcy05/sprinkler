#ifndef MYRTC_H
#define MYRTC_H

#include <RTClib.h>
#include <logger.h>

class RtcManager
{
public:
    RtcManager(RTC_DS3231 &rtcRef);
    void increaseOneHour();
    void increaseOneMinute();

    void setTimeFromCompile();

private:
    RTC_DS3231 &rtc;
};


#endif // MYRTC_H
