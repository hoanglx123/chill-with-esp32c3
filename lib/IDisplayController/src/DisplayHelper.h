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
    static IDisplayController* createDisplayController(const SCREEN_TYPE type, DISPLAY_SETTING_t settings)
    {
        switch (type) 
        {
            case SPI_OLED_128x64: 
            {
                return new SSD1306OledSPIController(settings.spiOledSettings);
            }

            case I2C_OLED_128x64:
            {
                return new SSD1306OledI2CController(settings.i2cSettings);
            }
            
            case SPI_TFT_160x80: 
            {
                return new ST7735TftSPIController(settings.spiTftSettings);
            }
            
            default:
            {
                return nullptr;
            }
        }
    }
};