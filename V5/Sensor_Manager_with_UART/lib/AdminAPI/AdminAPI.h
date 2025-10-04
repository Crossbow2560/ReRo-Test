#pragma once
#include <Arduino.h>
#include <SensorManager.h>
// Future
// ---------- AS7341 Colour sensor ----------
inline void readAS7341(int* out1, int *out2){
    SensorManager::AS7341Data snapshot1 = SensorManager::readAS7341_1();
    SensorManager::AS7341Data snapshot2 = SensorManager::readAS7341_2();
    // Load the data
    out1[0] = snapshot1.f1;
    out1[1] = snapshot1.f2;
}   
inline void get_colour(char *out){
    
}