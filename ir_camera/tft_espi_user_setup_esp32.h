

// Setup for the ESP32 S2 with ST7796S 480x320 SPI display
// Note SPI DMA with ESP32 S2 is not currently supported

// See SetupX_Template.h for all options available
#define ST7796_DRIVER

// #define ESP32
// esp32:esp32:featheresp32
// .\arduino-cli.exe compile --fqbn esp32:esp32:featheresp32 --library ..\libraries\ --port COM10 --upload --verbose 

#define TFT_MISO 19
#define TFT_MOSI 18 //23 //18 // 23
#define TFT_SCLK  5 //18 //5 // 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   25 // 2  // Data Command control pin
#define TFT_RST   4  // (A5) // Reset pin (could connect to RST pin)
//#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define TOUCH_CS 22     // Chip select pin (T_CS) of touch screen


#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

// FSPI port will be used unless the following is defined
// #define USE_HSPI_PORT // Commenting this out didn't affect the performance

// Using 35, 36, 37
// 60MHz + no HSPI => 16 FPS ; OPA Speed: 100% ;; retested later and it was 100% again.

// Using 11, 12, 13
//                                      LVGL benchmark:
// #define SPI_FREQUENCY  27000000   // LVGL benchmark: 13 FPS ; OPA Speed: 100%   ESP32-S2
// #define SPI_FREQUENCY  40000000   // LVGL benchmark: 16 FPS ; OPA Speed: 93%    ESP32-S2
// #define SPI_FREQUENCY  60000000   // LVGL benchmark: 16 FPS ; OPA Speed: 93%    ESP32-S2
#define SPI_FREQUENCY  60000000      // HSPI ; 18 FPS ; OPA Speed: 94%    ESP32
// #define SPI_FREQUENCY  80000000   // DOES NOT WORK

// #define SPI_READ_FREQUENCY  6000000 // 6 MHz is the maximum SPI read speed for the ST7789V

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000