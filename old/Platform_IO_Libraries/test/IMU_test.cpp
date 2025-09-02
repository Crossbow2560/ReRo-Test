#include<arduino.h>
#include "BMX160.h"

BMX imu;

void setup(){
    imu.init();
    Serial.begin(115200);
}
void loop(){
    uint8_t accel_x,accel_y,accel_z;
    uint8_t gyro_x,gyro_y,gyro_z;
    accel_x = imu.getAccelData()->x;
    accel_y = imu.getAccelData()->y;
    accel_z = imu.getAccelData()->z;
    gyro_x = imu.getGyroData()->x;
    gyro_y = imu.getGyroData()->y;
    gyro_z = imu.getGyroData()->z;
    printf("---------------------------------\n");
    Serial.printf("Accelerometer readings : %d,%d,%d\n",accel_x,accel_y,accel_z);
    Serial.printf("Gyroscope readings : %d,%d,%d\n",gyro_x,gyro_y,gyro_z);
    printf("---------------------------------\n");
    delay(500);
}