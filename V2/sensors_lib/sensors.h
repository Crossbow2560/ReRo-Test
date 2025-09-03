#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Arduino.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>

class TOF{
    public:
        TOF();
        uint8_t* readAllValues();
        // ~TOF();
    private:
        uint8_t _addrs[] = { 0x30, 0x31, 0x32, 0x33, 0x34 };
        static const int _count = 5;
        DFRobot_VL6180X* _tof;
        void begin();   // initial sensor array
        void init_TOF(uint8_t i); // initialize single sensor
        uint8_t values[_count] = {0, 0, 0, 0, 0};
        int _cePins[_count] = {2, 3, 6, 7, 10};      
};

class BMX{
    public:
        BMX();
        sBmx160SensorData_t* getGyroData();
        sBmx160SensorData_t* getAccelData();
    private:
        void init();
        DFRobot_BMX160 sensor;
        sBmx160SensorData_t Ogyro, Oaccel;
};

class IR{
    public:
        IR();
        int* getAllData();
    private:
        static const int _count = 8;
        int _values[_count] = {0, 0, 0, 0, 0, 0, 0, 0};
        int _pinArray[_count] = {12, 13, 14, 15, 16, 41, 40, 19};
};

#endif