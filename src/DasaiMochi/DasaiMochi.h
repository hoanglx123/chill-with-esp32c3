#pragma once

#include <vector>
#include <IBaseProgram.h>
#include <DisplayHelper.h>
#include <TouchController.h>
#include <TimerController.h>

class DasaiMochi : public IBaseProgram
{
private:
    using BUFFER_FRAME = std::vector<uint8_t>;
    using ANIMATION_FRAMES = std::vector<BUFFER_FRAME>;

public:
    DasaiMochi();
    ~DasaiMochi() override = default;

    void initProgram() override;
    void runProgram() override;

private:
    static std::vector<ANIMATION_FRAMES> createAnimationsSet();

    void onTouchEvent(TouchController::TOUCH_GESTURE gesture);
    void onTimerTimeout(TimerController::TIMER_ID timerId);

private:
    IDisplayController* mDisplayController;
    uint8_t             mCurrentFrameIndex = 0;

private:
    /* Current nimation */
    ANIMATION_FRAMES mCurrentDasaiMochiBitmap;

    /* All animations */
    PROGMEM std::vector<ANIMATION_FRAMES> mAllAnimations;

private: 
    static const uint8_t DISPLAY_FRAME_INTERVAL = 200U; /* ms */
};
