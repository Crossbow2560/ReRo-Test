#include <Arduino.h>
#include <SensorManager.h>
#include <AdminAPI.h>
#include <SensorManager.h>
#include <UserAPI.h>

int crash_detect(){
    int TOF_data[SensorManager::TOF_COUNT];
    readTOFs(TOF_data);

    for(int i = 0; i < SensorManager::TOF_COUNT; i++){
        if(TOF_data[i] == 0) continue; // skip invalid TOF reading
        if(TOF_data[i] <= TOF_THRESHOLD){
            return 1; // Crash detected
        }
    }
    return 0; // No crash
}

// ---------- AS7341 Colour sensor ----------
void readAS7341(int* out1, int *out2){
    SensorManager::AS7341Data snapshot1 = SensorManager::readAS7341_1();
    SensorManager::AS7341Data snapshot2 = SensorManager::readAS7341_2();
    // Load the data
    out1[0] = snapshot1.f1;
    out1[1] = snapshot1.f2;
}   

void crash_avoid(){
    // Write code her to reverse the motors 
}