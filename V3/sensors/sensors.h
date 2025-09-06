#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Arduino.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>

class TOF {
public:
    TOF();
    void begin();
    uint8_t readDistance(uint8_t idx);
private:
    int _isInitialized = 0;
    int _count = 5;
    int _cePins[_count] = {2, 3, 6, 7, 10};
    uint8_t _addrs[_count] = { 0x30, 0x31, 0x32, 0x33, 0x34 };
    DFRobot_VL6180X* _tof;
    void init_TOF(uint8_t i);
};