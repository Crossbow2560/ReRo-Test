#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>
#include <atomic>
#include <string.h>

class SensorManager {
public:
    static constexpr size_t TOF_COUNT = 5; // 3 on bus1, 2 on bus2
    
    struct TOFData {
        uint8_t tof[TOF_COUNT];  // distance in mm
    };

    struct IMUData {
        float accel[3];   // X, Y, Z m/s^2
        float gyro[3];    // X, Y, Z dps
        float magneto[3]; // X, Y, Z uT
    };

    // === Public API ===
    static TOFData readTOF_();
    static IMUData readBMX_();

private:
    friend void taskAdmin(void*);
    static void init_();
    static void updateTOF_();
    static void updateBMX_();

// ---------- I2C Buses ----------
    static TwoWire I2Cbus1_;   // SDA=34, SCL=38
    static TwoWire I2Cbus2_;   // SDA=47, SCL=48

// ---------- TOF sensors ----------
    // CE pins grouped per bus
    static const int cePinsBus1_[3];
    static const int cePinsBus2_[2];

    // I2C addresses grouped per bus
    static const uint8_t iicAddrsBus1_[3];
    static const uint8_t iicAddrsBus2_[2];
    
    // Flattened arrays for loop convenience
    static const int cePins_[TOF_COUNT];
    static const uint8_t iicAddrs_[TOF_COUNT];

    // Map each sensor to a bus
    static TwoWire* busMap_[TOF_COUNT];

    // Sensor object
    static DFRobot_VL6180X* sensors_[TOF_COUNT];
    
// ---------- BMX160 sensor ----------
    static DFRobot_BMX160* bmx_;

// ---------- Double buffers ----------
    static TOFData bufferTOFA_;
    static TOFData bufferTOFB_;
    static std::atomic<TOFData*> activeTOFBuffer_;

    static IMUData bufferBMXA_;
    static IMUData bufferBMXB_;
    static std::atomic<IMUData*> activeBMXBuffer_;
};
