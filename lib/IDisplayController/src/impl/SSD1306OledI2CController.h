#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "IDisplayController.h"  

class SSD1306OledI2CController : public IDisplayController
{
private:
    static const int SCREEN_ADDRESS = 0x3D; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

public:
    SSD1306OledI2CController() = delete;
    SSD1306OledI2CController(const I2C_OLED_SETTINGS_t& settings);
    ~SSD1306OledI2CController() override = default; 

    void init() override;
    void clearDisplay() override;
    void display() override;
    void drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void setTextSize(uint8_t s) override;
    void setTextColor(uint16_t c) override;
    void setCursor(int16_t x, int16_t y) override;
    void print(const char* text) override;

    virtual uint8_t getWidth() const override
    {
        return mI2CSettings.screenResolution.width;
    }

    virtual uint8_t getHeight() const override
    {
        return mI2CSettings.screenResolution.height;
    }

private: 
    Adafruit_SSD1306    mDisplayController;
    I2C_OLED_SETTINGS_t mI2CSettings;
};