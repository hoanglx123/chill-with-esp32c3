#include "SSD1306OledSPIController.h"  

SSD1306OledSPIController::SSD1306OledSPIController(const SPI_OLED_SETTINGS_t& settings)
{
    mSPISettings = settings;
    mDisplayController = Adafruit_SSD1306(mSPISettings.screenResolution.width, mSPISettings.screenResolution.height, &SPI, 
                                            mSPISettings.pinDC, mSPISettings.pinRESET, mSPISettings.pinSS);
}

void SSD1306OledSPIController::init()
{
    SPI.begin(mSPISettings.pinSCK, mSPISettings.pinMISO, mSPISettings.pinMOSI, mSPISettings.pinSS);

    if (!mDisplayController.begin(SSD1306_SWITCHCAPVCC)) 
    {
        for (;;); // hang if failed
    }
    mDisplayController.clearDisplay();
}

void SSD1306OledSPIController::clearDisplay()
{
    mDisplayController.clearDisplay();
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

void SSD1306OledSPIController::setTextColor(uint16_t c)
{
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
