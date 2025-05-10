#ifndef MYRTC_H
#define MYRTC_H

#include <RTClib.h>


class RtcManager{
    public:
        RtcManager();

        RTC_DS3231 rtc;

        void increase_hour();
        void increase_min();
};

extern RtcManager myRTC;


#endif //MYRTC_H
