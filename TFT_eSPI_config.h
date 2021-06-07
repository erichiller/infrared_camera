
// NOTE:

// Parallel bus is only supported for the STM32 and ESP32
// Example below is for ESP32 Parallel interface with UNO displays
// #define USE_HSPI_PORT
#define TFT_PARALLEL_8_BIT

// necessary because of https://esp32.com/viewtopic.php?t=18986
#define VSPI SPI2_HOST

// Tell the library to use 8 bit parallel mode (otherwise SPI is assumed)
#define ILI9481_DRIVER

// The ESP32 and TFT the pins used for testing are:
#define TFT_CS A3     // Chip select control pin (library pulls permanently low
#define TFT_DC A2     // Data Command control pin - must use a pin in the range 0-31
#define TFT_RST A4    // Reset pin, toggles on startup

#define TFT_WR A1    // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD A0    // Read strobe control pin

#define TFT_D0 13    // Must use pins in the range 0-31 for the data bus
#define TFT_D1 14    // so a single register write sets/clears all bits.
#define TFT_D2 7     // Pins can be randomly assigned, this does not affect
#define TFT_D3 8     // TFT screen update performance.
#define TFT_D4 9
#define TFT_D5 10
#define TFT_D6 11
#define TFT_D7 12




#define LOAD_GLCD     // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2    // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4    // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6    // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7    // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8    // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF    // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT