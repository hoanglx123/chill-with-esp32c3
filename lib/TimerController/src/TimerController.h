#pragma once

#include <stdint.h>
#include <functional>
#include <Arduino.h>

class TimerController
{
public:
    enum class TIMER_ID : uint8_t
    {
        UNKNOWN = 0U,
        TIMER0,
        TIMER1, 
        TIMER2
    };

    /* Callback */
    using TIMER_CALLBACK = std::function<void(TIMER_ID timerId)>;

public:
    static TimerController& getInstance()
    {
        static TimerController instance;
        return instance;
    }

    void initTimer(TIMER_ID timerId, uint64_t durationInMs, TIMER_CALLBACK callback);
    void startTimer();
    void stopTimer();

    TIMER_ID getTimerId() const;
    void invokeTimerCallback();

private:
    TimerController() = default;
    ~TimerController() = default;

private:
    TIMER_ID       mTimerId      = TIMER_ID::UNKNOWN;
    TIMER_CALLBACK mCallback     = nullptr;
    hw_timer_t*    mTimerInfo    = nullptr;
    uint64_t       mDurationInMs = 0U;

private:
    static const uint64_t NUMBER_OF_TICKS { 1000U }; // 1000 ticks = 1 milisecond
};