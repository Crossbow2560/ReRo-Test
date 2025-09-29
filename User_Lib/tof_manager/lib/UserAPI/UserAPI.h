#pragma once
#include <Arduino.h>
#include <TOFManager.h>

#define TOF_COUNT TOFManager::TOF_COUNT

inline void waitMS(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

inline void readTOFs(int* out) {
    TOFManager::TOFData snapshot = TOFManager::read();
    for (size_t i = 0; i < TOF_COUNT; i++) {
        out[i] = static_cast<int>(snapshot.tof[i]);
    }
}