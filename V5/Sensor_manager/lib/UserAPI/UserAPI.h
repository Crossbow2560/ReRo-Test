#pragma once
#include <Arduino.h>
#include <SensorManager.h>

#define TOF_COUNT SensorManager::TOF_COUNT

inline void waitMS(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

// --- TOF wrapper ---
inline void readTOFs(int* out) {
    SensorManager::TOFData snapshot = SensorManager::readTOF_();
    for (size_t i = 0; i < TOF_COUNT; i++) {
        out[i] = static_cast<int>(snapshot.tof[i]);
    }
}

// --- IMU wrappers ---
inline void readAccel(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.accel[0];
    out[1] = imu.accel[1];
    out[2] = imu.accel[2];
}

inline void readGyro(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.gyro[0];
    out[1] = imu.gyro[1];
    out[2] = imu.gyro[2];
}

inline void readMagneto(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.magneto[0];
    out[1] = imu.magneto[1];
    out[2] = imu.magneto[2];
}

// --- IR wrappers ---
inline void readIRArray(int* valuesOut) {
    SensorManager::IRData snapshot = SensorManager::readIR_(); // binary 0/1
    for (size_t i = 0; i < SensorManager::IR_COUNT; i++)
        valuesOut[i] = snapshot.values[i];
}

inline void readIRRawArray(int* valuesOut) {
    SensorManager::IRData snapshot = SensorManager::readIRRaw_();
    for (size_t i = 0; i < SensorManager::IR_COUNT; i++)
        valuesOut[i] = snapshot.values[i]; // raw RC readings
}
