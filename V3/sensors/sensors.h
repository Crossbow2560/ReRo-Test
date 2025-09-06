#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Arduino.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>

class TOF {
public:
    TOF();
    ~TOF();
    void begin();
    uint8_t readDistance(uint8_t idx);
    void readAllDistances(uint8_t* values);
private:
    int _isInitialized = 0;
    static const int _count = 5;
    int _cePins[_count] = {2, 3, 6, 7, 10};
    uint8_t _addrs[_count] = { 0x30, 0x31, 0x32, 0x33, 0x34 };
    DFRobot_VL6180X* _tof;
    void init_TOF(uint8_t i);
};

class BMX{
public:
    BMX();
    void begin();
    void getGyroData(sBmx160SensorData_t* gyroData);
    void getAccelData(sBmx160SensorData_t* accelData);
private:
    DFRobot_BMX160 bmx_sensor;
    int _isInitialized = 0;
    sBmx160SensorData_t Ogyro, Oaccel;
};

#endif
