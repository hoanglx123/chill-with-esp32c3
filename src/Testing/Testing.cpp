#include <vector>
#include "Testing.h"

#include "frames_auto_generated_01.h"

Testing::Testing()
{
    DISPLAY_SETTING_t settings;
    settings.spiTftSettings.pinMOSI  = SPI_MOSI;
    settings.spiTftSettings.pinMISO  = SPI_MISO;
    settings.spiTftSettings.pinSCK   = SPI_SCK;
    settings.spiTftSettings.pinSS    = SPI_SS;
    settings.spiTftSettings.pinDC    = TFT_DC;
    settings.spiTftSettings.pinRESET = TFT_RESET;
    settings.spiTftSettings.screenResolution.width  = SCREEN_SIZE_CONST::WIDTH_160;
    settings.spiTftSettings.screenResolution.height = SCREEN_SIZE_CONST::HEIGHT_80;

    /* Initialize Display controller */
    mDisplayController = DisplayHelper::createDisplayController(SCREEN_TYPE::SPI_TFT_160x80, settings);

    /* Setup frames */
    setupFrames();
}

void Testing::initProgram()
{
    mDisplayController->init();
    mDisplayController->clearDisplay();

    mDisplayController->setTextColor(TEXT_COLOR::COLOR_WHITE);
    mDisplayController->setTextSize(1);
    mDisplayController->setCursor(10, 30);
    mDisplayController->print("Hoang!");

    mDisplayController->drawRGBBitmap(0, 0, mCurrentDasaiMochiBitmap.at(0), 
                                        mDisplayController->getWidth(), mDisplayController->getHeight());
}

void Testing::runProgram()
{
    // mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mCurrentDasaiMochiBitmap.size();
    // mDisplayController->clearDisplay();
    // mDisplayController->drawRGBBitmap(0, 0, mCurrentDasaiMochiBitmap.at(mCurrentFrameIndex), 
    //                                     mDisplayController->getWidth(), mDisplayController->getHeight());
    // mDisplayController->display();

    // // Small delay to control speed
    // delay(100);  // 100ms
}

void Testing::setupFrames()
{
    mCurrentDasaiMochiBitmap = all_frames_01;
}