#include "sensors/sensors.h"

TOF tof;
BMX bmx;

uint8_t distanceValues[5];
sBmx160SensorData_t gyroData;
sBmx160SensorData_t accelData;


void setup(){
    Serial.begin(115200);
    tof.begin();
    bmx.begin();
}

void loop(){
    tof.readAllDistances(distanceValues);
    bmx.getGyroData(&gyroData);
    bmx.getAccelData(&accelData);

    // print all tof values
    for(int i = 0; i < 5; i++){
        Serial.print(distanceValues[i]);
        Serial.print(" ");
    }
    Serial.println("");


    // print all bmx values
    Serial.print("Gyro: ");
    Serial.print(gyroData.x); Serial.print(" ");
    Serial.print(gyroData.y); Serial.print(" ");
    Serial.print(gyroData.z); Serial.println();
    
    Serial.print("Accel: ");
    Serial.print(accelData.x); Serial.print(" ");
    Serial.print(accelData.y); Serial.print(" ");
    Serial.print(accelData.z); Serial.println();
}