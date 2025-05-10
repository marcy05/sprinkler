#include <RtcManager.h>
#include <logger.h>

RtcManager::RtcManager(){
    Wire.begin();
    if (!this->rtc.begin()) {
        Serial.println("RTC not found!");
        while (1);  // halt
    } else {
        debugln("RTC intialized.");
    }

    if (this->rtc.lostPower()) {
        Serial.println("RTC lost power, setting time to compile time.");
        //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void RtcManager::increase_hour(){
    DateTime now = this->rtc.now();

    this->rtc.adjust(DateTime(now.year(),
                              now.month(),
                              now.day(),
                              now.hour() + 1,
                              now.minute(), 0));
}

void RtcManager::increase_min(){
    DateTime now = this->rtc.now();

    this->rtc.adjust(DateTime(now.year(),
                              now.month(),
                              now.day(),
                              now.hour(),
                              now.minute() + 1, 0));
}


RtcManager myRTC = RtcManager();
