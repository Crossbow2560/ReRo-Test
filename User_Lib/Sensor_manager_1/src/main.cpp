#include <Arduino.h>
#include <HardwareSerial.h>
#include <SensorManager.h>
#include <motors.h>
#include <UserAPI.h>

/*
SensorManager::AS7341Data sensor1;
SensorManager::AS7341Data sensor2 = 
SensorManager::readAS7341_2();
*/

extern void userSetup();
extern void userLoop();

HardwareSerial AdminSerial(1);
const int AdminSerialRX = 17;
const int AdminSerialTX = 18;

TaskHandle_t handleUserTask = NULL;
TaskHandle_t handleAdminTask = NULL;

void taskAdmin(void* pvParameters) {
    SensorManager::init_main();

    for (;;) {
        SensorManager::updateTOF_();
        SensorManager::updateBMX_();
        SensorManager::updateIR_();
        //SensorManager::updateAS7341();   
        waitMS(100);
    }
}
// actas as a void loop for user
void taskUser(void* pvParameters) {
    for (;;) {
        userLoop();
    }
}

void setup() {
    AdminSerial.begin(115200, SERIAL_8N1, AdminSerialRX, AdminSerialTX);
    delay(100);
    AdminSerial.print("This is working here");
    userSetup();

    xTaskCreatePinnedToCore(taskAdmin, "taskAdmin", 4096, NULL, 2, &handleAdminTask, 0);
    xTaskCreatePinnedToCore(taskUser,  "taskUser",  4096, NULL, 1, &handleUserTask,  1);
}

void loop() {
    // Empty
}