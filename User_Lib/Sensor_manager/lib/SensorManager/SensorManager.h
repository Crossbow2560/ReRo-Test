#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>
#include <QTRSensors.h>
#include <atomic>
#include <string.h>

class SensorManager {
public:
    static constexpr size_t TOF_COUNT = 5; // 3 on bus1, 2 on bus2
    static constexpr size_t IR_COUNT  = 8; // IR Array channel count

    // Strustures to store sensor datas
    struct TOFData {
        uint8_t tof[TOF_COUNT];  // distance in mm
    };

    struct IMUData {
        float accel[3];   // X, Y, Z m/s^2
        float gyro[3];    // X, Y, Z dps
        float magneto[3]; // X, Y, Z uT
    };

    struct IRData {
        uint16_t values[IR_COUNT];  // raw calibrated or binary values
    };

    // === Public API ===
    static TOFData readTOF_();          
    static IMUData readBMX_();       
    static IRData  readIR_();        // 0/1 High or Low
    static IRData  readIRRaw_();     // raw calibrated 0 to 2500

private:
    // ---------- Private functions ----------
    friend void taskAdmin(void*);
    static void init_();
    static void updateTOF_();
    static void updateBMX_();
    static void updateIR_();

    // ---------- I2C Buses ----------
    static TwoWire I2Cbus1_;   // SDA=34, SCL=38
    static TwoWire I2Cbus2_;   // SDA=47, SCL=48

    // ---------- TOF sensors ----------
    // CE pins arrays
    static const int cePinsBus1_[3];
    static const int cePinsBus2_[2];
    static const int cePins_[TOF_COUNT];
    // I2C addresses arrays
    static const uint8_t iicAddrsBus1_[3];
    static const uint8_t iicAddrsBus2_[2];
    static const uint8_t iicAddrs_[TOF_COUNT];
    static TwoWire* busMap_[TOF_COUNT];
    static DFRobot_VL6180X* sensors_[TOF_COUNT]; // TOF Sensor object

    // ---------- BMX160 sensor ----------
    static DFRobot_BMX160* bmx_; // BMX Sensor object

    // ---------- IR array sensor ----------
    static QTRSensors qtr_; // IR Array object
    static constexpr uint16_t IR_THRESHOLD = 1500; // IR Raw calibrated Threshold Value

    // ---------- Double buffers ----------
    // TOF Double buffers
    static TOFData bufferTOFA_;
    static TOFData bufferTOFB_;
    static std::atomic<TOFData*> activeTOFBuffer_;
    // BMX Double buffers
    static IMUData bufferBMXA_;
    static IMUData bufferBMXB_;
    static std::atomic<IMUData*> activeBMXBuffer_;
    // IR Double buffers
    static IRData bufferIRA_;
    static IRData bufferIRB_;
    static std::atomic<IRData*> activeIRBuffer_;
};
