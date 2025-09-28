#include <Arduino.h>
#include "TouchController.h"

static const unsigned long DEBOUNCE_DELAY = 50; /* ms */ 

volatile unsigned long gLastInterruptTime = 0;

void IRAM_ATTR onTouchInterrupt() 
{
    /* Get current time */
    const unsigned long currentTime = millis();

    if(currentTime - gLastInterruptTime > DEBOUNCE_DELAY)
    {     
        if(digitalRead(TouchController::getInstance().getPin()) == HIGH) 
        {
            TouchController::getInstance().handleTouchEvents(TouchController::TOUCH_STATE::DETECTED, currentTime);
        } 
        else 
        {
            TouchController::getInstance().handleTouchEvents(TouchController::TOUCH_STATE::RELEASED, currentTime);
        }

        /* Store the last interrupt time */
        gLastInterruptTime = currentTime;
    }
}

void TouchController::initTouchPin(int pin)
{
    mPin = pin;
    pinMode(mPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(mPin), onTouchInterrupt, CHANGE);
}

void TouchController::registerTouchEvent(TOUCH_EVENT_CALLBACK callback)
{
    mCallback = callback;
}

void TouchController::handleTouchEvents(TOUCH_STATE state, unsigned long currentTimestamp)
{
    switch (state)
    {
        case TOUCH_STATE::DETECTED:
        {
            if(mTapCount == 0)
            {
                mFirstTapTime = currentTimestamp;
            }
            
            mTapCount++;
            mGesture = TOUCH_GESTURE::WAITING_FOR_TAP;
            break;
        }

        case TOUCH_STATE::RELEASED:
        {
            if(mGesture == TOUCH_GESTURE::WAITING_FOR_TAP)
            {
                /* Single tap */
                if(mTapCount == 1 && (currentTimestamp - mFirstTapTime > DOUBLE_TAP_GAP))
                {
                    mGesture = TOUCH_GESTURE::SINGLE_TAP;
                }
                else if(mTapCount == 2 && (currentTimestamp - mFirstTapTime <= DOUBLE_TAP_GAP))
                {
                    mGesture = TOUCH_GESTURE::DOUBLE_TAP;
                }
                else if (currentTimestamp - mFirstTapTime > SINGLE_TAP_GAP)
                {
                    /* Tap is stuck. Waiting for next action */
                    mTapCount     = 0;
                    mFirstTapTime = 0;
                    mGesture      = TOUCH_GESTURE::NONE;
                }
            }
            break;
        }   
    
        default:
        {
            break;
        }
    }

    /* Invoke callback if touch gesture is valid */
    if( mGesture == TOUCH_GESTURE::SINGLE_TAP || 
        mGesture == TOUCH_GESTURE::DOUBLE_TAP)
    {
        if(mCallback)
        {
            mCallback(mGesture);
        }

        mTapCount     = 0;
        mFirstTapTime = 0;
        mGesture      = TOUCH_GESTURE::NONE;
    }
}