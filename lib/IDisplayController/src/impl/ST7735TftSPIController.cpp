#include <SPI.h>
#include <Adafruit_GFX.h>
#include "ST7735TftSPIController.h"  

ST7735TftSPIController::ST7735TftSPIController(const SPI_TFT_SETTINGS_t& settings) : mDisplayController(settings.pinSS, settings.pinDC, settings.pinRESET)
{
    mSpiTFTSettings = settings;
}

void ST7735TftSPIController::init()
{
    /* Initialize SPI */ 
    SPI.begin(mSpiTFTSettings.pinSCK, mSpiTFTSettings.pinMISO, mSpiTFTSettings.pinMOSI, mSpiTFTSettings.pinSS);
    SPI.setFrequency(SPI_FREQUENCY::SPI_FREQUENCY_40MHZ);  // 20MHz SPI (safe; test up to 40MHz)
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    /* Initialize TFT with correct init routine for 160x80 */ 
    mDisplayController.initR(INITR_MINI160x80);  // For 160x80 ST7735 (verify with your display)
    mDisplayController.setRotation(1);           // Adjust rotation: 0-3 (1 = landscape, 160x80)

    /* Clear the display */
    clearDisplay();
}

void ST7735TftSPIController::clearDisplay()
{
    mDisplayController.fillScreen(ST7735_BLACK);
}

void ST7735TftSPIController::display()
{
    // mDisplayController.display();
}

void ST7735TftSPIController::drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.drawBitmap(x, y, bitmap.data(), w, h, color);
}

void ST7735TftSPIController::drawRGBBitmap(int16_t x, int16_t y, std::vector<uint16_t> bitmap, int16_t w, int16_t h)
{
    mDisplayController.drawRGBBitmap(x, y, bitmap.data(), w, h);
}

void ST7735TftSPIController::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.fillRect(x, y, w, h, color);
}

void ST7735TftSPIController::setTextSize(uint8_t s)
{
    mDisplayController.setTextSize(s);
}

void ST7735TftSPIController::setTextColor(uint16_t c)
{
    mDisplayController.setTextColor(c);
}

void ST7735TftSPIController::setCursor(int16_t x, int16_t y)
{
    mDisplayController.setCursor(x, y);
}

void ST7735TftSPIController::print(const char* text)
{
    mDisplayController.print(text);
}
