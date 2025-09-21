#include <SPI.h>
#include <Adafruit_GFX.h>
#include "SSD1306OledSPIController.h"  

SSD1306OledSPIController::SSD1306OledSPIController(const SPI_OLED_SETTINGS_t& settings)
{
    mSpiOledSettings = settings;
    mDisplayController = Adafruit_SSD1306(mSpiOledSettings.screenResolution.width, mSpiOledSettings.screenResolution.height, &SPI, 
                                            mSpiOledSettings.pinDC, mSpiOledSettings.pinRESET, mSpiOledSettings.pinSS);
}

void SSD1306OledSPIController::init()
{
    SPI.begin(mSpiOledSettings.pinSCK, mSpiOledSettings.pinMISO, mSpiOledSettings.pinMOSI, mSpiOledSettings.pinSS);

    if (!mDisplayController.begin(SSD1306_SWITCHCAPVCC)) 
    {
        for (;;); // hang if failed
    }
    mDisplayController.clearDisplay();
}

void SSD1306OledSPIController::clearDisplay()
{
    mDisplayController.clearDisplay();

    // Default settings after clear
    this->setTextSize(1);
    this->setTextColor(TEXT_COLOR::COLOR_WHITE);
}

void SSD1306OledSPIController::display()
{
    mDisplayController.display();
}

void SSD1306OledSPIController::drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.drawBitmap(x, y, bitmap.data(), w, h, color);
}

void SSD1306OledSPIController::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.fillRect(x, y, w, h, color);
}

void SSD1306OledSPIController::setTextSize(uint8_t s)
{
    mDisplayController.setTextSize(s);
}

void SSD1306OledSPIController::setTextColor(TEXT_COLOR color)
{
    uint16_t c;
    switch (color)
    {
        case TEXT_COLOR::COLOR_WHITE:
        {
            c = SSD1306_WHITE;
            break;
        }
        
        case TEXT_COLOR::COLOR_BLACK:
        {
            c = SSD1306_BLACK;
            break;
        }

        case TEXT_COLOR::COLOR_INVERSE:
        {
            c = SSD1306_INVERSE;
            break;
        }

        default:
        {
            c = SSD1306_WHITE;
            break;
        }
    }

    mDisplayController.setTextColor(c);
}

void SSD1306OledSPIController::setCursor(int16_t x, int16_t y)
{
    mDisplayController.setCursor(x, y);
}

void SSD1306OledSPIController::print(const char* text)
{
    mDisplayController.print(text);
}
