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

std::vector<uint8_t> DisplayHelper::compressRLE(const std::vector<uint8_t>& rawInput)
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

std::vector<uint8_t> DisplayHelper::decompressRLE(const std::vector<uint8_t>& compressedInput)
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

std::vector<uint8_t> DisplayHelper::compressDelta(const std::vector<uint8_t>& previousFullFrame, const std::vector<uint8_t>& currentFullFrame)
{
    std::vector<uint8_t> output;

    for (size_t i = 0; i < currentFullFrame.size(); ++i) 
    {
        if(currentFullFrame.at(i) != previousFullFrame.at(i))
        {
            /* Index (2 bytes), value (1 byte) */ 
            output.push_back(static_cast<uint8_t>(i & 0xFF));
            output.push_back(static_cast<uint8_t>((i >> 8) & 0xFF));
            output.push_back(currentFullFrame.at(i));
        }
    }

    return output;
}

std::vector<uint8_t> DisplayHelper::decompressDelta(const std::vector<uint8_t>& currentFullFrame, const std::vector<uint8_t>& nextCompressedFrame)
{
    std::vector<uint8_t> output = currentFullFrame;

    for (size_t i = 0; i + 2 < nextCompressedFrame.size(); i += 3) 
    {
        /* Get position of compressed pixel */
        uint16_t index = static_cast<uint16_t>(nextCompressedFrame.at(i)) | (static_cast<uint16_t>(nextCompressedFrame.at(i + 1)) << 8);
        /* Get value of  compressed pixel */
        uint8_t  value = nextCompressedFrame.at(i + 2);

        if (index < output.size()) 
        {
            output.at(index) = value;
        }
    }

    return output;
}