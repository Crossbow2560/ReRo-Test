#include <UserAPI.h>
#define TOF_COUNT 5
void userSetup() {
    userserial.println("In user setup function");
    waitMS(100);
}

void userLoop() {
    int tofValues[TOF_COUNT];
    readTOFs(tofValues);

    userserial.println("TOF readings:");
    for (int i = 0; i < TOF_COUNT; i++) {
        userserial.printf("  TOF[%d] = %d\n", i, tofValues[i]);
    }
    float accel[3], gyro[3], mag[3];  

    readAccel(accel);
    readGyro(gyro);
    readMagneto(mag);
    userserial.println("-------IMU Data-------");
    userserial.printf("Accel: X=%.2f Y=%.2f Z=%.2f m/s^2\n", accel[0], accel[1], accel[2]);
    userserial.printf("Gyro : X=%.2f Y=%.2f Z=%.2f g\n", gyro[0], gyro[1], gyro[2]);
    userserial.printf("Mag  : X=%.2f Y=%.2f Z=%.2f uT\n",  mag[0], mag[1], mag[2]);
  
    int irBinary[8];
    int irRaw[8];
    readIRArray(irBinary);     
    readIRRawArray(irRaw);      
    userserial.println("IR Sensor Readings:");
    for (int i = 0; i < 8; i++) {
        userserial.printf("[%d] Binary: %d | Raw: %d\n", i, irBinary[i], irRaw[i]);
    }
    

waitMS(50);

}