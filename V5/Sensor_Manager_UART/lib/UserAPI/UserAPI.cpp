#include <UserAPI.h>
#include <Arduino.h>
#include <SensorManager.h>
#include <motors.h>
#include <cstdarg>
#define TOF_COUNT SensorManager::TOF_COUNT

void waitMS(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

static motor myMotors;

// --- User UART Functions ----
UserSerial userserial;

void UserSerial::print(const char* msg) {
    Serial.print(msg);
}

void UserSerial::println(const char* msg) {
    Serial.println(msg);
}

void UserSerial::printf(const char* fmt, ...) {
    char buf[256];  // adjust size if needed
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Serial.print(buf);
}

// --- TOF wrapper ---
void readTOFs(int* out) {
    SensorManager::TOFData snapshot = SensorManager::readTOF_();
    for (size_t i = 0; i < TOF_COUNT; i++) {
        out[i] = static_cast<int>(snapshot.tof[i]);
    }
}

// --- IMU wrappers ---
void readAccel(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.accel[0];
    out[1] = imu.accel[1];
    out[2] = imu.accel[2];
}

void readGyro(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.gyro[0];
    out[1] = imu.gyro[1];
    out[2] = imu.gyro[2];
}

void readMagneto(float* out) {
    SensorManager::IMUData imu = SensorManager::readBMX_();
    out[0] = imu.magneto[0];
    out[1] = imu.magneto[1];
    out[2] = imu.magneto[2];
}

// --- IR wrappers ---
void readIRArray(int* valuesOut) {
    SensorManager::IRData snapshot = SensorManager::readIR_(); // binary 0/1
    for (size_t i = 0; i < SensorManager::IR_COUNT; i++)
        valuesOut[i] = snapshot.values[i];
}

void readIRRawArray(int* valuesOut) {
    SensorManager::IRData snapshot = SensorManager::readIRRaw_();
    for (size_t i = 0; i < SensorManager::IR_COUNT; i++)
        valuesOut[i] = snapshot.values[i]; // raw RC readings
}

// -- Motors wrappers ---
void motors_init() {
    myMotors.motors_init();
}

void setLeftSpeed(int direction, int value) {
    myMotors.setLeftSpeed(direction, value);
}

void setRightSpeed(int direction, int value) {
    myMotors.setRightSpeed(direction, value);
}