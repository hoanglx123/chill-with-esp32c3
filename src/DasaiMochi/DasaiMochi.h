#pragma once

#include <IBaseProgram.h>
#include <DisplayHelper.h>
#include "config.h"

class DasaiMochi : public IBaseProgram {
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

private:
    using BUFFER_FRAME = std::vector<uint8_t>;

    // 128x64 pixel bitmap for "Dasai Mochi" (black/white)
    std::vector<BUFFER_FRAME> PROGMEM mDasaiMochiBitmap;
};
