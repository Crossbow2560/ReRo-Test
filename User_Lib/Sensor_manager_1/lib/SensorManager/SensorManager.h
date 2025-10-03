#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>
#include <QTRSensors.h>
#include <DFRobot_AS7341.h>
#include <atomic>
#include <string.h>

// Issues
// TOFCOUNT -1 to TOFCOUNT
class SensorManager {
public:
    static constexpr size_t TOF_COUNT = 5;      // 3 on bus1, 2 on bus2
    static constexpr size_t IR_COUNT  = 8;      // IR Array channel count
    static constexpr size_t AS7341_COUNT = 2;   // AS7341 sensors count
    // Structures to store sensor datas
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

    struct AS7341Data {
        uint16_t f1;       // Channel 1 
        uint16_t f2;       // Channel 2
        uint16_t f3;       // Channel 3
        uint16_t f4;       // Channel 4
        uint16_t f5;       // Channel 5
        uint16_t f6;       // Channel 6
        uint16_t f7;       // Channel 7
        uint16_t f8;       // Channel 8
        uint16_t clear;    // Clear channel
        uint16_t nir;      // Near IR
    };

    // === Public API ===
    static TOFData readTOF_();          
    static IMUData readBMX_();       
    static IRData  readIR_();         // 0/1 High or Low
    static IRData  readIRRaw_();      // raw calibrated 0 to 2500
    static AS7341Data readAS7341_1(); // Sensor on I2Cbus1
    static AS7341Data readAS7341_2(); // Sensor on I2Cbus2

private:
    // ---------- Private functions ----------
    friend void taskAdmin(void*);
    // Sensor initialization functions
    static void init_main();
    static void init_I2C();
    static void init_TOF();
    static void init_BMX();
    static void init_IR();
    static void init_AS7341();

    // Sensor updation functions
    static void updateTOF_();
    static void updateBMX_();
    static void updateIR_();
    static void updateAS7341_1_();
    static void updateAS7341_2_();
    static void updateAS7341();
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
    static TwoWire* busMapTOF_[TOF_COUNT];          // Maps each TOF sensor to its I2C bus (I2Cbus1_ or I2Cbus2_)
    static DFRobot_VL6180X* sensors_[TOF_COUNT];    // TOF Sensor object

    // ---------- BMX160 sensor ----------
    static DFRobot_BMX160* bmx_; // BMX Sensor object

    // ---------- IR array sensor ----------
    static QTRSensors qtr_; // IR Array object
    static constexpr uint16_t IR_THRESHOLD = 1500; // IR Raw calibrated Threshold Value
    
    // ---------- AS7341 sensor ----------
    static TwoWire* busMapAS7341_[AS7341_COUNT];    // Maps each AS7431 sensor to its I2C bus (I2Cbus1_ or I2Cbus2_)
    static DFRobot_AS7341* as7341_[AS7341_COUNT];   // AS7341 Sensor object
    static constexpr uint8_t LED_BRIGHTNESS = 10;   // Values form 1 to 19 
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
    // AS7341 Double buffers
    static AS7341Data bufferAS7341_1A_;
    static AS7341Data bufferAS7341_1B_;
    static std::atomic<AS7341Data*> activeAS7341_1Buffer_;
    static AS7341Data bufferAS7341_2A_;
    static AS7341Data bufferAS7341_2B_;
    static std::atomic<AS7341Data*> activeAS7341_2Buffer_;
};
