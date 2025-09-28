#include <vector>
#include "config.h"
#include "DasaiMochi.h"

// Auto-generated frames
#include "frames_auto_generated_cell_2_0.h"
#include "frames_auto_generated_cell_2_1.h"

DasaiMochi::DasaiMochi()
{
    DISPLAY_SETTING_t settings;
    settings.spiOledSettings.pinMOSI  = SPI_MOSI;
    settings.spiOledSettings.pinMISO  = SPI_MISO;
    settings.spiOledSettings.pinSCK   = SPI_SCK;
    settings.spiOledSettings.pinSS    = SPI_SS;
    settings.spiOledSettings.pinDC    = OLED_DC;
    settings.spiOledSettings.pinRESET = OLED_RESET;
    settings.spiOledSettings.screenResolution.width  = SCREEN_SIZE_CONST::WIDTH_128;
    settings.spiOledSettings.screenResolution.height = SCREEN_SIZE_CONST::HEIGHT_64;

    /* Initialize Display controller */
    mDisplayController = DisplayHelper::createDisplayController(SCREEN_TYPE::SPI_OLED_128x64, settings);

    /* Setup touch sensor */
    TouchController::getInstance().initTouchPin(TOUCH_PIN);
    TouchController::getInstance().registerTouchEvent([this](TouchController::TOUCH_GESTURE guester)
    {
        this->onTouchEvent(guester);
    });

    /* Initialize timer */
    TimerController::getInstance().initTimer(TimerController::TIMER_ID::TIMER0, DISPLAY_FRAME_INTERVAL, ([this](TimerController::TIMER_ID timerId)
    {
        this->onTimerTimeout(timerId);
    }));
}

void DasaiMochi::initProgram()
{
    /* Display something */
    mDisplayController->init();
    mDisplayController->clearDisplay();
    mDisplayController->setTextSize(2);
    mDisplayController->setTextColor(TEXT_COLOR::COLOR_WHITE);
    mDisplayController->setCursor(0, 0);
    mDisplayController->print("Hello");
    mDisplayController->display();

    delay(500);  // 500ms

    /* Initialize animations */
    mAllAnimations = DasaiMochi::createAnimationsSet();

    /* Start timer */
    TimerController::getInstance().startTimer();
}

void DasaiMochi::runProgram()
{
    // delay(100);  // 100ms
}

void DasaiMochi::onTouchEvent(TouchController::TOUCH_GESTURE gesture)
{
    switch (gesture)
    {
        case TouchController::TOUCH_GESTURE::SINGLE_TAP: 
        {
            mCurrentFrameIndex = 0;
            mCurrentDasaiMochiBitmap = mAllAnimations.at(0);
            break;
        }

        case TouchController::TOUCH_GESTURE::DOUBLE_TAP:
        {
            mCurrentFrameIndex = 0;
            mCurrentDasaiMochiBitmap = mAllAnimations.at(1);
            break;
        }
    }
}

void DasaiMochi::onTimerTimeout(TimerController::TIMER_ID timerId)
{
    switch(timerId)
    {
        case TimerController::TIMER_ID::TIMER0:
        {
            if(mCurrentDasaiMochiBitmap.empty() == false)
            {
                /* Clear screen */
                mDisplayController->clearDisplay();
                
                /* Display new frame */
                mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mCurrentDasaiMochiBitmap.size();
                const std::vector<uint8_t> decompressedFrame = DisplayHelper::decompressRLE(mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex));
                // const std::vector<uint8_t> decompressedFrame = mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex);
                mDisplayController->drawBitmap(0, 0, decompressedFrame, 128, 64, SSD1306_WHITE);
                mDisplayController->display();
            }
            break;
        }
    
        default:
        {
            break;
        }
    }
}

std::vector<DasaiMochi::ANIMATION_FRAMES> DasaiMochi::createAnimationsSet()
{
    const std::vector<ANIMATION_FRAMES> allRawAnimations
    {
        frame_cell_2_0::all_frames,
        frame_cell_2_1::all_frames
    };
    
    return allRawAnimations;

    // std::vector<ANIMATION_FRAMES> allCompressedAnimations;
    // for(const auto& animationTemp : allRawAnimations)
    // {
    //     /* All frames */
    //     std::vector<BUFFER_FRAME> compressedFrames;
    
    //     /* Compress frames */
    //     for(auto& frame : animationTemp)
    //     {
    //         compressedFrames.push_back(DisplayHelper::compressRLE(frame));
    //     }

    //     allCompressedAnimations.push_back(compressedFrames);
    // }

    // return allCompressedAnimations;
}