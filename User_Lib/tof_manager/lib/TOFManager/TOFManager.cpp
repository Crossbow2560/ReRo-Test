#include "TOFManager.h"

// TwoWire instances
TwoWire TOFManager::I2Cbus1_{1};
TwoWire TOFManager::I2Cbus2_{0};

// CE pins per bus
const int TOFManager::cePinsBus1_[3]   = {2, 3, 6};
const int TOFManager::cePinsBus2_[2]   = {7, 10};

// I2C addresses per bus
const uint8_t TOFManager::iicAddrsBus1_[3] = {0x30, 0x31, 0x32};
const uint8_t TOFManager::iicAddrsBus2_[2] = {0x33, 0x34};

// Flattened arrays (bus1 + bus2)
const int TOFManager::cePins_[TOF_COUNT]   = {2, 3, 6, 7, 10};
const uint8_t TOFManager::iicAddrs_[TOF_COUNT] = {0x30, 0x31, 0x32, 0x33, 0x34};

// Bus assignment map
TwoWire* TOFManager::busMap_[TOF_COUNT] = {
    &I2Cbus1_, &I2Cbus1_, &I2Cbus1_,
    &I2Cbus2_, &I2Cbus2_
};

// Sensor objects (allocated in init_)
DFRobot_VL6180X* TOFManager::sensors_[TOF_COUNT];

// Double buffers
TOFManager::TOFData TOFManager::bufferA_;
TOFManager::TOFData TOFManager::bufferB_;
std::atomic<TOFManager::TOFData*> TOFManager::activeBuffer_{&TOFManager::bufferA_};

// ------------------------ Implementation ------------------------

void TOFManager::init_() {
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
}

void TOFManager::update_() {
    // Pick write buffer
    TOFData* writeBuf =
        (activeBuffer_.load(std::memory_order_relaxed) == &bufferA_) ? &bufferB_ : &bufferA_;

    // Poll all sensors
    for (size_t i = 0; i < TOF_COUNT; i++) {
        writeBuf->tof[i] = sensors_[i]->rangePollMeasurement();
    }

    // Swap buffers
    activeBuffer_.store(writeBuf, std::memory_order_release);
}

TOFManager::TOFData TOFManager::read() {
    TOFData snapshot;
    TOFData* src = activeBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(TOFData));
    return snapshot;
}
