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
const int AdminSerialRX = 18;
const int AdminSerialTX = 17;

TaskHandle_t handleUserTask = NULL;
TaskHandle_t handleAdminTask = NULL;

// Initialization safety
volatile bool sensorsReady = false;

void taskAdmin(void* pvParameters) {
    SensorManager::init_main();

    for (;;) {
        SensorManager::updateTOF_();
        SensorManager::updateBMX_();
        SensorManager::updateIR_();        
        //SensorManager::updateAS7341(); 
        sensorsReady = true;
        AdminSerial.println("data has been updated");  
        waitMS(20);
    }
}
// actas as a void loop for user
void taskUser(void* pvParameters) {
    while (!sensorsReady) { vTaskDelay(50); }
    for (;;) {
        userLoop();
    }
}

void setup() {
    AdminSerial.begin(115200, SERIAL_8N1, AdminSerialRX, AdminSerialTX);
    Serial.begin(115200);
    delay(100);
    AdminSerial.print("This is working here");
    userSetup();

    xTaskCreatePinnedToCore(taskAdmin, "taskAdmin", 4096, NULL, 2, &handleAdminTask, 0);
    xTaskCreatePinnedToCore(taskUser,  "taskUser",  4096, NULL, 1, &handleUserTask,  1);
}

void loop() {
    // Empty
}