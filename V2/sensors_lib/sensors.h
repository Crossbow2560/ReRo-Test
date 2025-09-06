#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Arduino.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>

// Main sensor manager class
class sensor{
    public:
    sensor();
    
    private:
};

class TOF{
    friend class Sensor;
    public:
        TOF();
        uint8_t readAllValues();
    private:
        static const int _count = 5;
        uint8_t _addrs[_count] = { 0x30, 0x31, 0x32, 0x33, 0x34 };
        DFRobot_VL6180X* _tof;
        void begin();   // initial sensor array
        void init_TOF(uint8_t i); // initialize single sensor
        uint8_t values[_count] = {0, 0, 0, 0, 0};
        int _cePins[_count] = {2, 3, 6, 7, 10};      
};

class BMX{
    friend class Sensor;
    public:
        BMX();
        sBmx160SensorData_t* getGyroData();
        sBmx160SensorData_t* getAccelData();
    private:
        void init();
        DFRobot_BMX160 bmx;
        sBmx160SensorData_t Ogyro, Oaccel;
};

class IR{
    friend class Sensor;
    public:
        IR();
        int* getAllData();
    private:
        static const int _count = 8;
        int _values[_count] = {0, 0, 0, 0, 0, 0, 0, 0};
        int _pinArray[_count] = {12, 13, 14, 15, 16, 41, 40, 19};
};

#endif
