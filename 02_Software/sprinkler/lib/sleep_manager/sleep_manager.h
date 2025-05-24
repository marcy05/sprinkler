#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <Arduino.h>
#include <hw_abstraction.h>
#include <logger.h>
#include <esp_sleep.h>

void setupDeepSleep();
void setupDeepSleepWakeupAfterMins(uint16_t minutes);

#endif // SLEEP_MANAGER_H