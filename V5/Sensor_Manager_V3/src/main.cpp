#include <Arduino.h>
#include <HardwareSerial.h>
#include <SensorManager.h>
#include <motors.h>
#include <UserAPI.h>
#include <AdminAPI.h>
/*
SensorManager::AS7341Data sensor1;
SensorManager::AS7341Data sensor2 = 
SensorManager::readAS7341_2();
*/

// external functions declaration from user_code.cpp
extern void userSetup();
extern void userLoop();

// Function prototypes
void killUserTask();

// Define Admin UART instance 
HardwareSerial AdminSerial(1);
const int AdminSerialRX = 18;
const int AdminSerialTX = 17;

// Define task handles for all teh tasks
TaskHandle_t handleUserTask = NULL;
TaskHandle_t handlereadSensorsTask = NULL;
TaskHandle_t handleSecurityTask = NULL;

// Initialization safety
volatile bool sensorsReady = false;

// Poll sensors and update values in double buffers
void taskReadSensors(void* pvParameters) {
    SensorManager::init_main();

    // Discard first few TOF readings to recieve accurate sensors data
    const int discardCount = 5;
    int dummyTOF[SensorManager::TOF_COUNT];
    for (int i = 0; i < discardCount; i++) {
        SensorManager::updateTOF_();
        SensorManager::updateBMX_();
        SensorManager::updateIR_();  
        readTOFs(dummyTOF);
        vTaskDelay(20);
    }

    sensorsReady = true;  // only now mark sensors ready

    for (;;) {
        SensorManager::updateTOF_();
        SensorManager::updateBMX_();
        SensorManager::updateIR_();        
        //SensorManager::updateAS7341(); 
        waitMS(20);
    }
}


// Security Implementation
void taskSecurity(void* pvParameters) {
    for (;;) {  // infinite loop
        
        if (crash_detect()) {
            AdminSerial.println("UserLoop terminated");
            killUserTask();   
            AdminSerial.println("1 Bot trying to crash");
            break;
        }
        else if(digitalRead(47)){
            AdminSerial.println("UserLoop terminated");
            killUserTask();   
            AdminSerial.println("2 Manual reset from RPI");
            break;
        }
        vTaskDelay(10); // check every 10 ms
    }
    // Terminate all other tasks and the security task
    vTaskDelete(handlereadSensorsTask);
    vTaskDelete(NULL);
}

// acts as void loop for user
void taskUser(void* pvParameters) {
    while (!sensorsReady) { vTaskDelay(50); } // Wait until accurate sensor readings come
    for (;;) {
        userLoop();
    }
}

void setup() {
    // RPI Manual reset pin
    pinMode(47,INPUT);
    // Initialize and start User and Admin UARTs
    AdminSerial.begin(115200, SERIAL_8N1, AdminSerialRX, AdminSerialTX); // Admin Serial monitor on UART 1 on 17(TX),18(RX) 
    Serial.begin(115200);   // User Serial Monitor on the UART USB port on ESP32 
    delay(100);
    // Call the user setup function
    userSetup();
    AdminSerial.println("Setup successful");

    // Initialize and start FREERTOS Tasks
    xTaskCreatePinnedToCore(taskReadSensors, "taskReadSensors", 4096, NULL, 2, &handlereadSensorsTask, 0);
    xTaskCreatePinnedToCore(taskUser,  "taskUser",  4096, NULL, 1, &handleUserTask,  1);
    xTaskCreatePinnedToCore(taskSecurity, "taskSecurity", 4096, NULL,  3,&handleSecurityTask , 0);

}

void loop() {
    // Empty
}

void killUserTask(){
    if (handleUserTask != NULL) {
        vTaskDelete(handleUserTask);  // stop user task
        handleUserTask = NULL;
    }
    crash_avoid();
}
