#pragma once

#include <stdint.h>
#include <functional>

class TouchController
{
public: 
    enum class TOUCH_STATE : uint8_t
    {
        IDLE = 0U,
        DETECTED,
        RELEASED,
    };

    enum class TOUCH_GESTURE : uint8_t
    {
        NONE = 0U,
        WAITING_FOR_TAP,
        SINGLE_TAP,
        DOUBLE_TAP,
        LONG_PRESS,
    };

    /* Callback */
    using TOUCH_EVENT_CALLBACK = std::function<void(TOUCH_GESTURE gesture)>;

public:
    static TouchController& getInstance()
    {
        static TouchController instance;
        return instance;
    }

    void initTouchPin(int pin);
    void registerTouchEvent(TOUCH_EVENT_CALLBACK callback);
    void handleTouchEvents(TOUCH_STATE state, unsigned long currentTimestamp);

    int getPin() const { return mPin; }

private:
    TouchController() = default;
    ~TouchController() = default;

private:
    int                  mPin          = -1;
    int                  mTapCount     = 0;
    unsigned long        mFirstTapTime = 0;
    unsigned long        mLastTapTime  = 0;
    TOUCH_EVENT_CALLBACK mCallback     = nullptr;
    TOUCH_STATE          mLastState    = TOUCH_STATE::IDLE;
    TOUCH_GESTURE        mGesture      = TOUCH_GESTURE::NONE;

private:
    static const unsigned long DOUBLE_TAP_GAP = 250; /* ms */ 
    static const unsigned long SINGLE_TAP_GAP = 300; /* ms */ 
};