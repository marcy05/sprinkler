#ifndef SW_TIMER_H
#define SW_TIMER_H


class TimeHandler {
    public:
        static unsigned long minutesWakeupPeriod;
        static unsigned long wakeupTimestamp;
    
    public:
        TimeHandler();
        bool isWakeupTimeExpired();
};


extern TimeHandler systemTimeHandler;

#endif // SW_TIMER_H