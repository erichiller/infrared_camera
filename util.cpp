#include <Arduino.h>

void printhex(uint8_t val) {
    if (val < 0x10) Serial.print("0");
    Serial.print(val, HEX);
}
