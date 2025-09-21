#include <vector>
#include "DasaiMochi.h"
#include "config.h"

// Auto-generated frames
// #include "frames_auto_generated_01.h"
// #include "frames_auto_generated_02.h"

#define TOUCH_PIN 21

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
    pinMode(TOUCH_PIN, INPUT);
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
    // int touchState = digitalRead(TOUCH_PIN);

    // // if (touchState == HIGH)
    // // {
    // //     mCurrentDasaiMochiBitmap = all_frames_01;
    // // }
    // // else
    // // {
    // //     mCurrentDasaiMochiBitmap = all_frames_02;
    // // }

    // mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mCurrentDasaiMochiBitmap.size();
    // mDisplayController->clearDisplay();
    // mDisplayController->drawBitmap(0, 0, mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex), 128, 64, SSD1306_WHITE);
    // mDisplayController->display();

    // // Small delay to control speed
    // delay(100);  // 100ms
}

void DasaiMochi::setupFrames()
{
    // mCurrentDasaiMochiBitmap = all_frames_01;
}