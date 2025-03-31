#include <sleep_manager.h>
#include "esp_sleep.h"
#include <Arduino.h>
#include <hw_abstraction.h>
#include <logger.h>


void setupDeepSleep(){

    pinMode(Buttons::DEEP_SLEEP_BUTTON.PIN, INPUT_PULLUP);

    esp_sleep_enable_ext1_wakeup((1ULL << Buttons::DEEP_SLEEP_BUTTON.PIN), ESP_EXT1_WAKEUP_ALL_LOW);
    debugln("Configured push button for Deep sleep.");
}

void setupDeepSleepWakeupAfterMins(uint16_t minutes){
    uint64_t WAKEUP_INTERVAL_US = minutes * 60 * 1000000ULL;  // minutes
    esp_sleep_enable_timer_wakeup(WAKEUP_INTERVAL_US);
}
