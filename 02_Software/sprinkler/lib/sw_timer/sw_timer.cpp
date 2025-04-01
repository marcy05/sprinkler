#include <sw_timer.h>
#include <Arduino.h>

unsigned long TimeHandler::minutesWakeupPeriod = 1;
unsigned long TimeHandler::wakeupTimestamp = 0;

TimeHandler::TimeHandler(){
    this->deepSleepWakeupAfterMinutes = 15;
};

bool TimeHandler::isWakeupTimeExpired(){
    unsigned long durationMillis = this -> minutesWakeupPeriod * 60 * 1000;
    return (millis() - this->wakeupTimestamp >= durationMillis);
}


TimeHandler systemTimeHandler = TimeHandler();

