/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "./mlx90640/MLX90640_API.h"
#include "./mlx90640/MLX90640_I2C_Driver.h"

const uint8_t MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8 //Default shift for MLX90640 in open air

static float mlx90640To[768];
paramsMLX90640 mlx90640;

//file: main.cpp
// #include "Arduino.h"

// void setup(){
//   Serial.begin(115200);
// }



void app_main(void)
{
    printf("MLX90640 IR Array Example");

//   if (isConnected() == false)
//   {
//     printf("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
//     while (1);
//   }
//   printf("MLX90640 online!");

  //Get device parameters - We only have to do this once
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0){
      printf("Failed to load eeprom");
  }

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0){
    printf("Parameter extraction failed");
  }




    // /* Print chip information */
    // esp_chip_info_t chip_info;
    // esp_chip_info(&chip_info);
    // printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
    //         CONFIG_IDF_TARGET,
    //         chip_info.cores,
    //         (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
    //         (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    // printf("silicon revision %d, ", chip_info.revision);

    // printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
    //         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // printf("Free heap: %d\n", esp_get_free_heap_size());

    for (int i = 30; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

//Returns true if the MLX90640 is detected on the I2C bus
// bool isConnected()
// {
//   Wire.beginTransmission((uint8_t)MLX90640_address);
//   if (Wire.endTransmission() != 0)
//     return (false); //Sensor did not ACK
//   return (true);
// }