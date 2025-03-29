#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <Arduino.h>


void setupDeepSleep();
void setupDeepSleepWakeupAfterMins(uint16_t minutes);


#endif // SLEEP_MANAGER_H