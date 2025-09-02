#ifndef VL6180X_H
#define VL6180X_H
#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>

class TOF {
public:
    TOF(const int* cePins, const uint8_t* addrs, size_t count);
    uint8_t values[] = {0, 0, 0, 0, 0};
    void begin();
    uint8_t readDistance(uint8_t idx);
    uint8_t* readAllValues();

private:
    int* _cePins;
    uint8_t* _addrs;
    size_t _count;
    DFRobot_VL6180X* _tof;
    void init_TOF(uint8_t i);
};

#endif