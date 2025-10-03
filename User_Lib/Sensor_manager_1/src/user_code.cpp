#include <UserAPI.h>

void userSetup() {
    Serial.begin(115200);
    Serial.println("In user setup function");
    delay(100);
}

void userLoop() {
    int tofValues[TOF_COUNT];
    readTOFs(tofValues);

    Serial.println("TOF readings:");
    for (size_t i = 0; i < TOF_COUNT; i++) {
        Serial.printf("  TOF[%d] = %d\n", (int)i, tofValues[i]);
    }
    float accel[3], gyro[3], mag[3];  

    readAccel(accel);
    readGyro(gyro);
    readMagneto(mag);
    Serial.println("-------IMU Data-------");
    Serial.printf("Accel: X=%.2f Y=%.2f Z=%.2f m/s^2\n", accel[0], accel[1], accel[2]);
    Serial.printf("Gyro : X=%.2f Y=%.2f Z=%.2f g\n", gyro[0], gyro[1], gyro[2]);
    Serial.printf("Mag  : X=%.2f Y=%.2f Z=%.2f uT\n",  mag[0], mag[1], mag[2]);
  
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