#pragma once

#include <IBaseProgram.h>
#include <DisplayHelper.h>
#include <TouchController.h>

class DasaiMochi : public IBaseProgram
{
private:
    enum ANIMATION_SPEED
    {
        SPEED_FAST = 50,
        SPEED_MEDIUM = 100,
        SPEED_SLOW = 200
    };

    enum ANIMATION_TYPE
    {
        ANIMATION_01 = 1,
        ANIMATION_02 = 0,
    };

public:
    DasaiMochi();
    ~DasaiMochi() override = default;

    void initProgram() override;
    void runProgram() override;

private:
    void setupFrames();
    void onTouchEvent(TouchController::TOUCH_GESTURE gesture);

private:
    IDisplayController* mDisplayController;
    uint8_t             mCurrentFrameIndex = 0;
    ANIMATION_TYPE      mAnimationType     = ANIMATION_01;

    bool isChanged = false;

private:
    using BUFFER_FRAME = std::vector<uint8_t>;

    PROGMEM std::vector<BUFFER_FRAME> mCurrentDasaiMochiBitmap;
};
