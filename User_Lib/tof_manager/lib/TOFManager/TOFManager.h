#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>
#include <atomic>
#include <string.h>

class TOFManager {
public:
    static constexpr size_t TOF_COUNT = 5;  // 3 on bus1, 2 on bus2

    struct TOFData {
        uint8_t tof[TOF_COUNT];
    };

    static TOFData read();

private:
    friend void taskAdmin(void*);
    static void init_();
    static void update_();

    // --- Multi-bus support ---
    static TwoWire I2Cbus1_;   // SDA=34, SCL=38
    static TwoWire I2Cbus2_;   // SDA=47, SCL=48

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

    // --- Sensor objects ---
    // Need pointers so we can construct with (addr, bus)
    static DFRobot_VL6180X* sensors_[TOF_COUNT];

    // --- Double buffering ---
    static TOFData bufferA_;
    static TOFData bufferB_;
    static std::atomic<TOFData*> activeBuffer_;
};
