#include "sensors.h"

// =================== VL6180X Sensor Functions =================== // 

void TOF::init_TOF(uint8_t i) {
    digitalWrite(_cePins[i], HIGH);
    delay(10);
    while (!_tof[i].begin()) {
        Serial.print("Error Initializing TOF ");
        Serial.print(i);
        Serial.println();
    }
    _tof[i].setIICAddr(_addrs[i]);
    delay(50);
}

void TOF::begin() {
    for (size_t i = 0; i < _count; ++i) {
        pinMode(_cePins[i], OUTPUT);
        digitalWrite(_cePins[i], LOW);
    }
    delay(10);
    for (size_t i = 0; i < _count; ++i) init_TOF(i);
}

TOF::TOF(){
    // _tof = new DFRobot_VL6180X[_count];
    // begin();
}

uint8_t* TOF::readAllValues(){
    for(int i = 0; i < _count; i++){
        values[i] = _tof[i].rangePollMeasurement();
    }
    return values;
}

// =================== VL6180X Sensor Functions =================== // 

// =================== BMX160 Class Functions =================== // 

void BMX::init() {
    bmx.begin();
}

BMX::BMX() {
    bmx.init();
}

sBmx160SensorData_t* BMX::getGyroData() {
    bmx.getAllData(NULL, &Ogyro, NULL);
    return &Ogyro;
}

sBmx160SensorData_t* BMX::getAccelData() {
    bmx.getAllData(NULL, NULL, &Oaccel);
    return &Oaccel;
}

// =================== BMX160 Class Functions =================== //

// =================== IR Array Class Functions =================== //

IR::IR(){
    for(int i = 0; i < _count; i++){
        pinMode(_pinArray[i], INPUT);
    }
}

int* IR::getAllData(){
    for(int i = 0; i < _count; i++){
        _values[i] = digitalRead(_pinArray[i]);
    }
    return _values;
}

// =================== IR Array Class Functions =================== //