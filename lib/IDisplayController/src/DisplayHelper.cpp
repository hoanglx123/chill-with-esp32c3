#include "DisplayHelper.h"

IDisplayController* DisplayHelper::createDisplayController(const SCREEN_TYPE type, DISPLAY_SETTING_t settings)
{
    switch (type) 
    {
        case SCREEN_TYPE::SPI_OLED_128x64: 
        {
            return new SSD1306OledSPIController(settings.spiOledSettings);
        }

        case SCREEN_TYPE::I2C_OLED_128x64:
        {
            return new SSD1306OledI2CController(settings.i2cSettings);
        }
        
        case SCREEN_TYPE::SPI_TFT_160x80: 
        {
            return new ST7735TftSPIController(settings.spiTftSettings);
        }
        
        default:
        {
            return nullptr;
        }
    }
}

std::vector<uint8_t> DisplayHelper::compressRLE(const std::vector<uint8_t> rawInput)
{
    std::vector<uint8_t> output;

    /* Check if input is valid */
    if (rawInput.empty() == false) 
    {
        uint8_t count = 1;
        uint8_t prev  = rawInput[0];

        for (size_t i = 1; i < rawInput.size(); ++i) 
        {
            if (rawInput[i] == prev && count < 255) 
            {
                ++count;
            } 
            else 
            {
                output.push_back(count);
                output.push_back(prev);
                prev  = rawInput[i];
                count = 1;
            }
        }

        /* Add the last run */ 
        output.push_back(count);
        output.push_back(prev);
    }

    return output;
}

std::vector<uint8_t> DisplayHelper::decompressRLE(const std::vector<uint8_t> compressedInput)
{
    std::vector<uint8_t> output;

    /* Check if compressedInput is valid */
    if (compressedInput.empty() == false) 
    {
        for (size_t i = 0; i < compressedInput.size(); i += 2) 
        {
            if (i + 1 < compressedInput.size()) 
            {
                uint8_t count = compressedInput[i];
                uint8_t value = compressedInput[i + 1];

                for (uint8_t j = 0; j < count; ++j) 
                {
                    output.push_back(value);
                }
            }
        }
    }

    return output;
}
