#include <vector>
#include "config.h"
#include "DasaiMochi.h"

// Auto-generated frames
#include "frames_auto_generated_cell_0_1.h"
#include "frames_auto_generated_cell_2_0.h"
#include "frames_auto_generated_cell_2_1.h"
// #include "frames_auto_generated_cell_3_0.h"

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
    mDisplayController->print("Booting...");
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
            mDisplayController->setTextSize(2);
            mDisplayController->setTextColor(TEXT_COLOR::COLOR_WHITE);
            mDisplayController->setCursor(0, 0);
            mDisplayController->display();
            delay(500);  // 500ms

            /* Update animation */
            mCurrentFrameIndex = 0;
            mCurrentAnimationIndex = (mCurrentAnimationIndex + 1) % mAllAnimations.size();
            mCurrentDasaiMochiBitmap = mAllAnimations.at(mCurrentAnimationIndex++);
            break;
        }

        case TouchController::TOUCH_GESTURE::DOUBLE_TAP:
        {
            /* Stop timer */
            TimerController::getInstance().stopTimer();

            mDisplayController->clearDisplay();
            mDisplayController->setTextSize(2);
            mDisplayController->setTextColor(TEXT_COLOR::COLOR_WHITE);
            mDisplayController->setCursor(0, 0);
            mDisplayController->print("Resetting ...");
            mDisplayController->display();
            delay(500);  // 500ms

            /* Start timer */
            TimerController::getInstance().startTimer();
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
                std::vector<uint8_t> decompressedFrame;

                /* Clear screen */
                mDisplayController->clearDisplay();
                
                /* Display new frame */
                mCurrentFrameIndex = mCurrentFrameIndex % mCurrentDasaiMochiBitmap.size();
                
                if(mCurrentFrameIndex == 0)
                {
                    /* RLE decompress for first frame */
                    decompressedFrame = DisplayHelper::decompressRLE(mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex));
                }
                else             
                {
                    /* Delta decompress with previous frame */ 
                    decompressedFrame = DisplayHelper::decompressDelta(mCurrentFrameBuffer, mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex));
                }
                
                /* Store decompressed frame */
                mCurrentFrameBuffer = decompressedFrame;
                mCurrentFrameIndex++; 

                /* Frame is valid */
                if(decompressedFrame.empty() == false)
                {
                    /* Display frame */
                    mDisplayController->drawBitmap(0, 0, decompressedFrame, 128, 64, SSD1306_WHITE);
                    mDisplayController->display();
                }
            }
            else
            {
                mCurrentFrameIndex = 0;
                mCurrentAnimationIndex = 0;
                mCurrentDasaiMochiBitmap = mAllAnimations.at(0);
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
        frame_cell_0_1::all_frames,
        frame_cell_2_0::all_frames,
        frame_cell_2_1::all_frames,
        // frame_cell_3_0::all_frames,
    };
    
    return allRawAnimations;
}