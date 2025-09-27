#include <Arduino.h>
#include "TimerController.h"

void IRAM_ATTR onTimer() 
{
    /* Invoke callback */
    TimerController::getInstance().invokeTimerCallback();
}

void TimerController::initTimer(TIMER_ID timerId, uint64_t durationInMs, TIMER_CALLBACK callback)
{
    /* Store timer information */
    mTimerId      = timerId;
    mCallback     = callback;
    mDurationInMs = durationInMs;

    /* Initialize timer */ 
    mTimerInfo = timerBegin(static_cast<uint8_t>(mTimerId), 80, true);  // Timer 0, prescaler 80 (1µs/tick at 80 MHz), count up
    timerAttachInterrupt(mTimerInfo, &onTimer, true);                   // Attach ISR, auto-reload
    timerAlarmWrite(mTimerInfo, NUMBER_OF_TICKS * mDurationInMs, true); // 1,000,000 ticks = 1 second
}

void TimerController::startTimer()
{
    if(mTimerInfo != nullptr)
    {
        timerAlarmEnable(mTimerInfo);  // Enable timer
    }
}

void TimerController::stopTimer()
{
    if(mTimerInfo != nullptr)
    {
        timerAlarmDisable(mTimerInfo);  // Disable timer
    }
}

TimerController::TIMER_ID TimerController::getTimerId() const
{
    return mTimerId;
}

void TimerController::invokeTimerCallback()
{
    /* Stop timer */
    stopTimer();

    /* Invoke callback */
    if(mCallback)
    {
        mCallback(mTimerId);
    }

    /* Start timer again */
    startTimer();
}