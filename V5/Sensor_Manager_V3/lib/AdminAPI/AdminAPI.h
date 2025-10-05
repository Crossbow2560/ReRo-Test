#ifndef ADMINAPI_H
#include <Arduino.h>
#include <SensorManager.h>
#include <UserAPI.h>

static constexpr int TOF_THRESHOLD = 20;

// --- Crash detect ---
int crash_detect();
void crash_avoid();

// Future
void readAS7341(int* out1, int *out2);
#define ADMINAPI_H
#endif

