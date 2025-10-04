#ifndef USERAPI_H
#define USERAPI_H
#include <stdint.h>


void waitMS(uint32_t ms);

// --- User UART Functions ----
class UserSerial {
public:
    void print(const char* msg);
    void println(const char* msg);
    void printf(const char* fmt, ...);
};
extern UserSerial userserial; 

// --- TOF wrapper ---
void readTOFs(int* out);

// --- IMU wrappers ---
void readAccel(float* out);
void readGyro(float* out);
void readMagneto(float* out);

// --- IR wrappers ---
void readIRArray(int* valuesOut);
void readIRRawArray(int* valuesOut);

// -- Motors wrappers ---
void motors_init();
void setLeftSpeed(int direction, int value);
void setRightSpeed(int direction, int value);

#endif