#pragma one 

#include "IDisplayController.h"
#include "impl/SSD1306OledSPIController.h"

struct DISPLAY_SETTING_t
{
    SPI_OLED_SETTINGS_t spiSettings;
    I2C_OLED_SETTINGS_t i2cSettings;
};

class DisplayHelper
{
public:
    static IDisplayController* createDisplayController(const SCREEN_TYPE type, DISPLAY_SETTING_t settings)
    {
        switch (type) {
            case SPI_OLED_128x64: {
                return new SSD1306OledSPIController(settings.spiSettings);
            }

            case I2C_OLED_128x64:
                // Implement I2C OLED controller creation if needed
            default:
                return nullptr;
        }
    }
};