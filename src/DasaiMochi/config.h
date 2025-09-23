#pragma once

// Your SPI pins (ESP32-C3 Super Mini default mapping)
#define OLED_CS     7   // SS
#define OLED_DC     1   // DC (⚠ UART TX, pick another if you want Serial debug)
#define OLED_RESET 10   // Reset

/* SPI Definition */
#define SPI_SCK     4
#define SPI_MISO    5
#define SPI_MOSI    6
#define SPI_SS      7

/* Touch pin */
#define TOUCH_PIN 21