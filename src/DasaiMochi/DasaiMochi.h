#pragma once

#include <IBaseProgram.h>
#include <DisplayHelper.h>

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

private:
    IDisplayController* mDisplayController;
    uint8_t mCurrentFrameIndex = 0;
    ANIMATION_TYPE mAnimationType = ANIMATION_01;
    bool isToogle = false;

private:
    using BUFFER_FRAME = std::vector<uint8_t>;

    // 128x64 pixel bitmap for "Dasai Mochi" (black/white)
    // std::vector<BUFFER_FRAME> PROGMEM mDasaiMochiBitmapType1;
    // std::vector<BUFFER_FRAME> PROGMEM mDasaiMochiBitmapType2;
    PROGMEM std::vector<BUFFER_FRAME> mCurrentDasaiMochiBitmap;
};
