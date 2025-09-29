#include <Arduino.h>
#include <HardwareSerial.h>
#include <SensorManager.h>
#include <UserAPI.h>

extern void userSetup();
extern void userLoop();

HardwareSerial AdminSerial(1);
const int AdminSerialRX = 17;
const int AdminSerialTX = 18;

TaskHandle_t handleUserTask = NULL;
TaskHandle_t handleAdminTask = NULL;

void taskAdmin(void* pvParameters) {
    SensorManager::init_();

    for (;;) {
        SensorManager::updateTOF_();
        SensorManager::updateBMX_();
        waitMS(50);
    }
}

void taskUser(void* pvParameters) {
    for (;;) {
        userLoop();
    }
}

void setup() {
    AdminSerial.begin(115200, SERIAL_8N1, AdminSerialRX, AdminSerialTX);
    delay(100);

    userSetup();

    xTaskCreatePinnedToCore(taskAdmin, "taskAdmin", 4096, NULL, 2, &handleAdminTask, 0);
    xTaskCreatePinnedToCore(taskUser,  "taskUser",  4096, NULL, 1, &handleUserTask,  1);
}

void loop() {
    // Empty
}