#include "bmx160.h"

BMX::BMX() {
}

void BMX::init() {
  sensor.begin();
}

sBmx160SensorData_t* BMX::getGyroData() {
  sensor.getAllData(NULL, &Ogyro, NULL);
  return &Ogyro;
}

sBmx160SensorData_t* BMX::getAccelData() {
  sensor.getAllData(NULL, NULL, &Oaccel);
  return &Oaccel;
}