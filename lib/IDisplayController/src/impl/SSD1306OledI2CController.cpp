#include "SSD1306OledI2CController.h"  

SSD1306OledI2CController::SSD1306OledI2CController(const I2C_OLED_SETTINGS_t& settings)
{
    mI2CSettings = settings;
    mDisplayController = Adafruit_SSD1306(mI2CSettings.screenResolution.width, mI2CSettings.screenResolution.height, &Wire, mI2CSettings.pinRESET);
}

void SSD1306OledI2CController::init()
{
    /* Initialize I2C */
    Wire.begin(mI2CSettings.pinSDA, mI2CSettings.pinSCL);
    Wire.setClock(400000);  // Fast I2C: 400kHz (default 100kHz; max ~1MHz for short wires)

    if (!mDisplayController.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
        for (;;); // hang if failed
    }
    mDisplayController.clearDisplay();
}

void SSD1306OledI2CController::clearDisplay()
{
    mDisplayController.clearDisplay();
}

void SSD1306OledI2CController::display()
{
    mDisplayController.display();
}

void SSD1306OledI2CController::drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.drawBitmap(x, y, bitmap.data(), w, h, color);
}

void SSD1306OledI2CController::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    mDisplayController.fillRect(x, y, w, h, color);
}

void SSD1306OledI2CController::setTextSize(uint8_t s)
{
    mDisplayController.setTextSize(s);
}

void SSD1306OledI2CController::setTextColor(uint16_t c)
{
    mDisplayController.setTextColor(c);
}

void SSD1306OledI2CController::setCursor(int16_t x, int16_t y)
{
    mDisplayController.setCursor(x, y);
}

void SSD1306OledI2CController::print(const char* text)
{
    mDisplayController.print(text);
}
