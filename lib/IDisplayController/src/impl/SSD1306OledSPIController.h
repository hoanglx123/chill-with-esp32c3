#pragma once

#include <Adafruit_SSD1306.h>
#include "IDisplayController.h"  

class SSD1306OledSPIController : public IDisplayController
{
public:
    SSD1306OledSPIController() = delete;
    SSD1306OledSPIController(const SPI_OLED_SETTINGS_t& settings);
    ~SSD1306OledSPIController() override = default; 

    void init() override;
    void clearDisplay() override;
    void display() override;
    void drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void setTextSize(uint8_t s) override;
    void setTextColor(TEXT_COLOR color) override;
    void setCursor(int16_t x, int16_t y) override;
    void print(const char* text) override;

    virtual uint8_t getWidth() const override
    {
        return mSpiOledSettings.screenResolution.width;
    }

    virtual uint8_t getHeight() const override
    {
        return mSpiOledSettings.screenResolution.height;
    }

private: 
    Adafruit_SSD1306    mDisplayController;
    SPI_OLED_SETTINGS_t mSpiOledSettings;
};