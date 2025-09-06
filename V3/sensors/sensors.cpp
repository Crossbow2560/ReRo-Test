#include "VL6180X.h"

// =================== VL6180X Sensor Functions =================== // 

TOF::TOF(){
    _tof = new DFRobot_VL6180X[_count];
}

void TOF::begin() {
    if(_isInitialized == 0){
        Wire.begin();
        for (size_t i = 0; i < _count; ++i) {
            pinMode(_cePins[i], OUTPUT);
            digitalWrite(_cePins[i], LOW);
        }
        delay(10);
        for (size_t i = 0; i < _count; ++i) init_TOF(i);
        _isInitialized = 1;
    }
    else{
        Serial.println("Already Initialized! ")
    }
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