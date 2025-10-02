#pragma once

#include <stdint.h>
#include <vector>

enum class SCREEN_TYPE : uint8_t 
{
    SPI_OLED_128x64,
    I2C_OLED_128x64,
    SPI_TFT_160x80,

    /* The last item */
    UNKNOWN_SCREEN
};

enum class TEXT_COLOR : uint_least8_t
{
    COLOR_WHITE = 0,
    COLOR_BLACK,
    COLOR_INVERSE
};

struct SCREEN_SIZE_CONST 
{
    /* Width */
    static const uint8_t WIDTH_128 = 128;
    static const uint8_t WIDTH_160 = 160;

    /* Height */
    static const uint8_t HEIGHT_64 = 64;
    static const uint8_t HEIGHT_80 = 80;
};

struct SCREEN_RESOLUTION_INFO_t 
{
    int8_t width;   // Width in pixels
    int8_t height;  // Height in pixels

    SCREEN_RESOLUTION_INFO_t()
        : width(SCREEN_SIZE_CONST::WIDTH_128), height(SCREEN_SIZE_CONST::HEIGHT_64) {}
};

struct SPI_OLED_SETTINGS_t 
{
    /* Revolution information */
    SCREEN_RESOLUTION_INFO_t screenResolution;
    
    /* Pin Configurations */
    int8_t  pinMISO  = -1;
    int8_t  pinMOSI  = -1;
    int8_t  pinSCK   = -1;
    int8_t  pinSS    = -1; // Chip Select pin (for SPI)
    int8_t  pinDC    = -1; // Data/Command pin
    int8_t  pinRESET = -1; // Reset pin
};

struct SPI_TFT_SETTINGS_t 
{
    /* Revolution information */
    SCREEN_RESOLUTION_INFO_t screenResolution;
    
    /* Pin Configurations */
    int8_t  pinMISO  = -1;
    int8_t  pinMOSI  = -1;
    int8_t  pinSCK   = -1;
    int8_t  pinSS    = -1; // Chip Select pin (for SPI)
    int8_t  pinDC    = -1; // Data/Command pin
    int8_t  pinRESET = -1; // Reset pin
    int8_t  pinBKL   = -1; // Reset BKL
};

struct I2C_OLED_SETTINGS_t 
{
    /* Revolution information */
    SCREEN_RESOLUTION_INFO_t screenResolution;
    int16_t width  = 128;
    int16_t height = 64;

    /* I2C Pin Configurations */
    int8_t pinSDA   = -1;
    int8_t pinSCL   = -1;
    int8_t pinRESET = -1;
};

struct SPI_FREQUENCY
{
    static const int32_t SPI_FREQUENCY_8MHZ  = 8000000;  // 8MHz
    static const int32_t SPI_FREQUENCY_16MHZ = 16000000; // 16MHz
    static const int32_t SPI_FREQUENCY_20MHZ = 20000000; // 20MHz
    static const int32_t SPI_FREQUENCY_40MHZ = 40000000; // 40MHz
};

struct FRAME_INFO_t 
{
    const uint16_t bytes_per_bitmap;
    const uint8_t* bitmap;
};

class IDisplayController
{
public:
    IDisplayController() = default;
    virtual ~IDisplayController() = default;

    virtual void init() = 0;
    virtual void clearDisplay() = 0;
    virtual void display() = 0;
    virtual void drawBitmap(int16_t x, int16_t y, std::vector<uint8_t> bitmap, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void drawRGBBitmap(int16_t x, int16_t y, std::vector<uint16_t> bitmap, int16_t w, int16_t h) {};
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void setTextSize(uint8_t s) = 0;
    virtual void setTextColor(TEXT_COLOR color) = 0;
    virtual void setCursor(int16_t x, int16_t y) = 0;
    virtual void print(const char* text) = 0;

    /* For RGB */

    virtual uint8_t getWidth() const = 0;
    virtual uint8_t getHeight() const = 0;
};