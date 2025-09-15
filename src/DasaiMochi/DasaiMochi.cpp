#include <vector>
#include "DasaiMochi.h"

// Auto-generated frames
#include "frames_auto_generated.h"

DasaiMochi::DasaiMochi()
{
    DISPLAY_SETTING_t settings;
    settings.spiSettings.pinMOSI  = SPI_MOSI;
    settings.spiSettings.pinMISO  = SPI_MISO;
    settings.spiSettings.pinSCK   = SPI_SCK;
    settings.spiSettings.pinSS    = SPI_SS;
    settings.spiSettings.pinDC    = OLED_DC;
    settings.spiSettings.pinRESET = OLED_RESET;
    settings.spiSettings.screenResolution.width  = SCREEN_SIZE_CONST::WIDTH_128;
    settings.spiSettings.screenResolution.height = SCREEN_SIZE_CONST::HEIGHT_64;

    /* Initialize Display controller */
    mDisplayController = DisplayHelper::createDisplayController(SCREEN_TYPE::SPI_OLED_128x64, settings);

    /* Setup frames */
    setupFrames();
}

void DasaiMochi::initProgram()
{
    mDisplayController->init();
    mDisplayController->clearDisplay();
}

void DasaiMochi::runProgram()
{
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mDasaiMochiBitmap.size();

    mDisplayController->clearDisplay();
    mDisplayController->drawBitmap(0, 0, mDasaiMochiBitmap.at(mCurrentFrameIndex), 128, 64, SSD1306_WHITE);
    mDisplayController->display();

    // Small delay to control speed
    delay(100);  // 100ms
}

void DasaiMochi::setupFrames()
{
    mDasaiMochiBitmap = all_frames;
}