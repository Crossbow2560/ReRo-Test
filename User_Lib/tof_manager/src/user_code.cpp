#include <UserAPI.h>

void userSetup() {
    Serial.begin(115200);
    delay(100);
}

void userLoop() {
    int tofValues[TOF_COUNT];
    readTOFs(tofValues);

    Serial.println("TOF readings:");
    for (size_t i = 0; i < TOF_COUNT; i++) {
        Serial.printf("  TOF[%d] = %d\n", (int)i, tofValues[i]);
    }

    waitMS(250);
}