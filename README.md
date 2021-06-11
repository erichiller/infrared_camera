# Infrared Camera

**Status**: Currently awaiting [this issue](https://github.com/espressif/arduino-esp32/issues/4729) to be fixed for `arduino-esp32`

## Hardware

* [Adafruit ESP32-S2 Metro](https://learn.adafruit.com/adafruit-metro-esp32-s2)
  * [Schematic](https://cdn-learn.adafruit.com/assets/assets/000/098/525/original/adafruit_products_MetroESP32S2_sch.png?1610394363)
  * [Pintout](https://cdn-learn.adafruit.com/assets/assets/000/102/126/original/adafruit_products_Adafruit_Metro_ESP32-S2_pinout.png)
* Adafruit MLX90640 55°
* MCUFriend TFT 480x320 - _this is a generic part, my controller is `ILI9481`_

## Pinout

LCD Pin     | ESP32-S2 Metro Pin
------------|----------------------
LCD_RD      | A0
LCD_WR      | A1
LCD_RS      | A2
LCD_CS      | A3
LCD_RST     | A4
X           | A5


LCD DataPin | ESP32-S2 Metro Pin
------------|----------------------
LCD_D7      | 12
LCD_D6      | 11
LCD_D5      | 10
LCD_D4      |  9
LCD_D3      |  8
LCD_D2      |  7



LCD Pin     | ESP32-S2 Metro Pin
------------|----------------------
SD_SCK      | 42
SD_DO       | 21
SD_DI       | 16
SD_SS       | 15
LCD_D1      | 14
LCD_D0      | 13

## Building with ESP-IDF

1. Download [ESP-IDF][^GettingStartedEspIdf] via git

        git clone --recursive https://github.com/espressif/esp-idf.git

2. Install ESP-IDF

        ./install.ps1

3. Activate environment

        ./export.ps1

4. Create new project, set chip to `esp32s2`

        idf.py set-target esp32s2

5. Configure project

        idf.py menuconfig

6. Must set `CONFIG_ESP_CONSOLE_USB_CDC=y` in `sdkconfig` (otherwise the [usb/serial port][^1] will not work)
7. Build

        idf.py build

8. Upload

        idf.py -p com4 flash

## Monitoring from the command line

    python (whereis miniterm.py) com7 115200

## Future Ideas

* Add a [camera](https://learn.adafruit.com/ttl-serial-camera) and overlay thermal/infradata on top of it


[^1]: https://github.com/espressif/arduino-esp32/blob/master/boards.txt#L3338
[^GettingStartedEspIdf]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html