#include "Bouncing.h"
#include <vector>

// Two sample 16x16 bitmaps (black/white)
static const std::vector<uint8_t> PROGMEM bmp1 {
    0xFF,0xFF,0x81,0x81,0xBD,0xBD,0xA5,0xA5,
    0xA5,0xA5,0xBD,0xBD,0x81,0x81,0xFF,0xFF,
    0xFF,0xFF,0x81,0x81,0xBD,0xBD,0xA5,0xA5,
    0xA5,0xA5,0xBD,0xBD,0x81,0x81,0xFF,0xFF
};

static const std::vector<uint8_t> PROGMEM bmp2 {
    0x00,0x3C,0x42,0x81,0x81,0xA5,0x99,0x81,
    0x81,0x99,0xA5,0x81,0x81,0x42,0x3C,0x00,
    0x00,0x3C,0x42,0x81,0x81,0xA5,0x99,0x81,
    0x81,0x99,0xA5,0x81,0x81,0x42,0x3C,0x00
};

Bouncing::Bouncing()
{
    DISPLAY_SETTING_t settings;
    settings.spiOledSettings.pinMOSI  = SPI_MOSI;
    settings.spiOledSettings.pinMISO  = SPI_MISO;
    settings.spiOledSettings.pinSCK   = SPI_SCK;
    settings.spiOledSettings.pinSS    = SPI_SS;
    settings.spiOledSettings.pinDC    = OLED_DC;
    settings.spiOledSettings.pinRESET = OLED_RESET;
    settings.spiOledSettings.screenResolution.width  = SCREEN_SIZE_CONST::WIDTH_128;
    settings.spiOledSettings.screenResolution.height = SCREEN_SIZE_CONST::HEIGHT_64;

    mDisplayController = DisplayHelper::createDisplayController(SCREEN_TYPE::SPI_OLED_128x64, settings);
}

void Bouncing::initProgram()
{
    mDisplayController->init();
    mDisplayController->clearDisplay();
}

void Bouncing::runProgram()
{
    // Clear screen
    mDisplayController->clearDisplay();

    // Draw two bitmaps
    mDisplayController->drawBitmap(x1, y1, bmp1, 16, 16, SSD1306_WHITE);
    mDisplayController->drawBitmap(x2, y2, bmp2, 16, 16, SSD1306_WHITE);

    // Update display once per frame
    mDisplayController->display();

    // Update positions
    x1 += dx1; y1 += dy1;
    x2 += dx2; y2 += dy2;

    // Bounce off edges
    if (x1 < 0 || x1 > (mDisplayController->getWidth() - 16)) dx1 = -dx1;
    if (y1 < 0 || y1 > (mDisplayController->getHeight() - 16)) dy1 = -dy1;
    if (x2 < 0 || x2 > (mDisplayController->getWidth() - 16)) dx2 = -dx2;
    if (y2 < 0 || y2 > (mDisplayController->getHeight()  - 16)) dy2 = -dy2;

    // Small delay to control speed
    delay(20);  // ~50 fps
}

