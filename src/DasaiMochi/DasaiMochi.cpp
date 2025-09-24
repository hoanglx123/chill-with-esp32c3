#include <vector>
#include "config.h"
#include "DasaiMochi.h"

// Auto-generated frames
#include "frames_auto_generated_wpgdt.h"
#include "frames_auto_generated_ivxpi.h"

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

    /* Setup frames */
    setupFrames();

    /* Setup touch sensor */
    TouchController::getInstance().initTouchPin(TOUCH_PIN);
    TouchController::getInstance().registerTouchEvent([this](TouchController::TOUCH_GESTURE guester)
    {
        this->onTouchEvent(guester);
    });
}

void DasaiMochi::initProgram()
{
    mDisplayController->init();
    mDisplayController->clearDisplay();
    mDisplayController->setTextSize(2);
    mDisplayController->setTextColor(TEXT_COLOR::COLOR_WHITE);
    mDisplayController->setCursor(0, 0);
    mDisplayController->print("Hello");
    mDisplayController->display();
}

void DasaiMochi::runProgram()
{
    if(mCurrentDasaiMochiBitmap.empty() == false)
    {
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mCurrentDasaiMochiBitmap.size();
        mDisplayController->clearDisplay();
        mDisplayController->drawBitmap(0, 0, mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex), 128, 64, SSD1306_WHITE);
        mDisplayController->display();
    
        // Small delay to control speed
        delay(100);  // 100ms
    }
}

void DasaiMochi::setupFrames()
{
    // mCurrentDasaiMochiBitmap = all_frames_01;
}

void DasaiMochi::onTouchEvent(TouchController::TOUCH_GESTURE gesture)
{
    switch (gesture)
    {
        case TouchController::TOUCH_GESTURE::SINGLE_TAP: 
        {
            mCurrentFrameIndex = 0;
            mCurrentDasaiMochiBitmap = frame_ivxpi::all_frames;
            break;
        }

        case TouchController::TOUCH_GESTURE::DOUBLE_TAP:
        {
            mCurrentFrameIndex = 0;
            mCurrentDasaiMochiBitmap = frame_wpgdt::all_frames;
            break;
        }

    }
}