#include "VL6180X.h"

#define N_SENSORS 5
int cePins[N_SENSORS] = {2, 3, 6, 7, 10};           // Chip enable pins for your sensors
uint8_t addrs[N_SENSORS] = {0x30, 0x31, 0x32, 0x33, 0x34};  // I2C addresses for your sensors

TOF sensors(cePins, addrs, N_SENSORS);

void setup() {
    Serial.begin(115200);
    sensors.begin();
    Serial.println("TOF sensors initialized!");
}

void loop() {
    for (int i = 0; i < N_SENSORS; i++) {
        uint8_t mm = sensors.readDistance(i);
        Serial.print("TOF");
        Serial.print(i);
        Serial.print(" (0x");
        Serial.print(addrs[i], HEX);
        Serial.print("): ");
        Serial.print(mm);
        Serial.println(" mm");
    }
    delay(500);
}