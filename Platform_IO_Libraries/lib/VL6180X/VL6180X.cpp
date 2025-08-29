#include "VL6180X.h"

TOF::TOF(const int* cePins, const uint8_t* addrs, size_t count) 
    : _count(count) {
    _cePins = new int[count];
    _addrs = new uint8_t[count];
    _tof = new DFRobot_VL6180X[count];
    for (size_t i = 0; i < count; ++i) {
        _cePins[i] = cePins[i];
        _addrs[i] = addrs[i];
    }
}

void TOF::begin() {
    Wire.begin();
    for (size_t i = 0; i < _count; ++i) {
        pinMode(_cePins[i], OUTPUT);
        digitalWrite(_cePins[i], LOW);
    }
    delay(10);
    for (size_t i = 0; i < _count; ++i) init_TOF(i);
}

void TOF::init_TOF(uint8_t i) {
    digitalWrite(_cePins[i], HIGH);
    delay(10);
    while (!_tof[i].begin()) {
        Serial.printf("Error Initializing TOF %d \n", i);
    }
    _tof[i].setIICAddr(_addrs[i]);
    delay(50);
}

uint8_t TOF::readDistance(uint8_t idx) {
    if (idx < _count) {
        return _tof[idx].rangePollMeasurement();
    }
    return 0;
}