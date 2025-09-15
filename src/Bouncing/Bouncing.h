#pragma once

#include <IBaseProgram.h>
#include <DisplayHelper.h>
#include "config.h"

class Bouncing : public IBaseProgram {
public:
    Bouncing();
    ~Bouncing() override = default;

    void initProgram() override;
    void runProgram() override;

private:
    IDisplayController* mDisplayController;

    int x1 = 0, y1 = 0, dx1 = 2, dy1 = 1;
    int x2 = 50, y2 = 30, dx2 = -1, dy2 = 2;
};
