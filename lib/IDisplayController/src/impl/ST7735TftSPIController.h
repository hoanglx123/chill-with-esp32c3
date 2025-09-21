#pragma once

#include <Adafruit_ST7735.h>
#include "IDisplayController.h"  

class ST7735TftSPIController : public IDisplayController
{
public:
    ST7735TftSPIController() = delete;
    ST7735TftSPIController(const SPI_TFT_SETTINGS_t& settings);
    ~ST7735TftSPIController() override = default; 

    void init() override;
    void clearDisplay() override;
    void display() override;
    void drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color) override;
    void drawRGBBitmap(int16_t x, int16_t y, std::vector<uint16_t> bitmap, int16_t w, int16_t h) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void setTextSize(uint8_t s) override;
    void setTextColor(TEXT_COLOR color) override;
    void setCursor(int16_t x, int16_t y) override;
    void print(const char* text) override;

    virtual uint8_t getWidth() const override
    {
        return mSpiTFTSettings.screenResolution.width;
    }

    virtual uint8_t getHeight() const override
    {
        return mSpiTFTSettings.screenResolution.height;
    }

private: 
    Adafruit_ST7735    mDisplayController;
    SPI_TFT_SETTINGS_t mSpiTFTSettings;
};