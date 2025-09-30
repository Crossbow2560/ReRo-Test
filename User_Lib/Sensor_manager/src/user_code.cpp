#include <UserAPI.h>
#include <QTRSensors.h>

void userSetup() {
    Serial.begin(115200);
    delay(100);
}

void userLoop() {
    int irBinary[8];
    int irRaw[8];
    readIRArray(irBinary);     
    readIRRawArray(irRaw);      
    Serial.println("IR Sensor Readings:");
    for (int i = 0; i < SensorManager::IR_COUNT; i++) {
        Serial.printf("[%d] Binary: %d | Raw: %d\n", i, irBinary[i], irRaw[i]);
    }

    waitMS(1000);
}
