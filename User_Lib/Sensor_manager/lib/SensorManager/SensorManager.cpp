#include "SensorManager.h"
#include <Arduino.h>

// TwoWire instances constructor initialization
TwoWire SensorManager::I2Cbus1_{1};
TwoWire SensorManager::I2Cbus2_{0};

// CE pins per bus
const int SensorManager::cePinsBus1_[3] = {2, 3, 6};
const int SensorManager::cePinsBus2_[2] = {7, 10};

// I2C addresses per bus
const uint8_t SensorManager::iicAddrsBus1_[3] = {0x30, 0x31, 0x32};
const uint8_t SensorManager::iicAddrsBus2_[2] = {0x33, 0x34};

// Flattened arrays (bus1 + bus2)
const int SensorManager::cePins_[TOF_COUNT]   = {2, 3, 6, 7, 10};
const uint8_t SensorManager::iicAddrs_[TOF_COUNT] = {0x30, 0x31, 0x32, 0x33, 0x34};

// Bus assignment map
TwoWire* SensorManager::busMap_[TOF_COUNT] = {
    &I2Cbus1_, &I2Cbus1_, &I2Cbus1_,
    &I2Cbus2_, &I2Cbus2_
};
// Sensor objects (allocated in init_)
DFRobot_VL6180X* SensorManager::sensors_[TOF_COUNT];
DFRobot_BMX160* SensorManager::bmx_;
// Double buffers
SensorManager::TOFData SensorManager::bufferTOFA_;
SensorManager::TOFData SensorManager::bufferTOFB_;
std::atomic<SensorManager::TOFData*> SensorManager::activeTOFBuffer_{&SensorManager::bufferTOFA_};

SensorManager::IMUData SensorManager::bufferBMXA_;
SensorManager::IMUData SensorManager::bufferBMXB_;
std::atomic<SensorManager::IMUData*> SensorManager::activeBMXBuffer_{&SensorManager::bufferBMXA_};

// ---------- Private Methods ----------
void SensorManager::init_() {
    // Init I2C buses
    I2Cbus1_.begin(34, 38);   // SDA=34, SCL=38
    I2Cbus2_.begin(47, 48);   // SDA=47, SCL=48

    // Reset all CE pins low
    for (size_t i = 0; i < TOF_COUNT; i++) {
        pinMode(cePins_[i], OUTPUT);
        digitalWrite(cePins_[i], LOW);
    }
    delay(10);

    // Init each sensor
    for (size_t i = 0; i < TOF_COUNT; i++) {
        // Create sensor object on correct bus with default address
        sensors_[i] = new DFRobot_VL6180X(0x29, busMap_[i]);

        // Enable this sensor
        digitalWrite(cePins_[i], HIGH);
        delay(10);

        // Try to begin until success
        while (!sensors_[i]->begin()) {
            Serial.printf("ERROR: TOF %d not initialized\n", (int)i);
            delay(1000);
        }

        // Assign unique I2C address
        sensors_[i]->setIICAddr(iicAddrs_[i]);
        delay(50);
    }

    // Initialize BMX160 on bus2
    bmx_ = new DFRobot_BMX160(&I2Cbus2_);
    if (!bmx_->begin()) {
        Serial.println("ERROR: BMX160 not initialized!");
        while (1);
    }
}

void SensorManager::updateTOF_() {
    TOFData* writeBuf =
        (activeTOFBuffer_.load(std::memory_order_relaxed) == &bufferTOFA_) ? &bufferTOFB_ : &bufferTOFA_;

    for (size_t i = 0; i < TOF_COUNT; i++) {
        writeBuf->tof[i] = sensors_[i]->rangePollMeasurement();
    }

    activeTOFBuffer_.store(writeBuf, std::memory_order_release);
}

void SensorManager::updateBMX_() {
    IMUData* writeBuf =
        (activeBMXBuffer_.load(std::memory_order_relaxed) == &bufferBMXA_) ? &bufferBMXB_ : &bufferBMXA_;

    sBmx160SensorData_t accel, gyro, mag;
    bmx_->getAllData(&mag, &gyro, &accel);

    writeBuf->accel[0] = accel.x; writeBuf->accel[1] = accel.y; writeBuf->accel[2] = accel.z;
    writeBuf->gyro[0]  = gyro.x;  writeBuf->gyro[1]  = gyro.y;  writeBuf->gyro[2]  = gyro.z;
    writeBuf->magneto[0] = mag.x; writeBuf->magneto[1] = mag.y; writeBuf->magneto[2] = mag.z;

    activeBMXBuffer_.store(writeBuf, std::memory_order_release);
}

SensorManager::TOFData SensorManager::readTOF_() {
    TOFData snapshot;
    TOFData* src = activeTOFBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(TOFData));
    return snapshot;
}

SensorManager::IMUData SensorManager::readBMX_() {
    IMUData snapshot;
    IMUData* src = activeBMXBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(IMUData));
    return snapshot;
}
