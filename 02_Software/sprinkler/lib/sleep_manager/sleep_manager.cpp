#include <sleep_manager.h>

void setupDeepSleep()
{

    pinMode(Buttons::DEEP_SLEEP_BUTTON.PIN, INPUT_PULLUP);

    esp_sleep_enable_ext1_wakeup((1ULL << Buttons::DEEP_SLEEP_BUTTON.PIN), ESP_EXT1_WAKEUP_ALL_LOW);
    debugln("SLM-Configured push button to wakeup from deep sleep.");
}

void setupDeepSleepWakeupAfterMins(uint16_t minutes)
{
    uint64_t WAKEUP_INTERVAL_US = minutes * 60 * 1000000ULL; // minutes
    esp_sleep_enable_timer_wakeup(WAKEUP_INTERVAL_US);
    debug("SLM-Wakeup timer from sleep in: ");
    debug(WAKEUP_INTERVAL_US);
    debugln(" ms");
}
