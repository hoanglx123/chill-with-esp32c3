#pragma one 

#include "IDisplayController.h"
#include "impl/SSD1306OledSPIController.h"
#include "impl/SSD1306OledI2CController.h"
#include "impl/ST7735TftSPIController.h"

struct DISPLAY_SETTING_t
{
    SPI_OLED_SETTINGS_t spiOledSettings;
    SPI_TFT_SETTINGS_t  spiTftSettings;
    I2C_OLED_SETTINGS_t i2cSettings;
};

class DisplayHelper
{
public:
    static IDisplayController* createDisplayController(const SCREEN_TYPE type, DISPLAY_SETTING_t settings);

    /* Run length encoding/decoding */
    static std::vector<uint8_t> compressRLE(const std::vector<uint8_t> rawInput);
    static std::vector<uint8_t> decompressRLE(const std::vector<uint8_t> compressedInput);
};