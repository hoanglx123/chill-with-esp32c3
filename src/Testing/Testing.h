#pragma once

#include <IBaseProgram.h>
#include <DisplayHelper.h>
#include "config.h"

class Testing : public IBaseProgram
{
    enum ANIMATION_SPEED
    {
        SPEED_FAST   = 50,
        SPEED_MEDIUM = 100,
        SPEED_SLOW   = 200
    };

    enum ANIMATION_TYPE
    {
        ANIMATION_01 = 1,
        ANIMATION_02 = 0,
    };

public:
    Testing();
    ~Testing() override = default;

    void initProgram() override;
    void runProgram() override;

private:
    void setupFrames();

private:
    IDisplayController* mDisplayController;
    ANIMATION_TYPE      mAnimationType     = ANIMATION_01;
    uint8_t             mCurrentFrameIndex = 0;
    bool isToogle = false;

private:
    using BUFFER_FRAME = std::vector<uint16_t>;

    PROGMEM std::vector<BUFFER_FRAME> mCurrentDasaiMochiBitmap;
};
