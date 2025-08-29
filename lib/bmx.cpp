#include "bmx.h"

BMX :: BMX(){
}

void BMX :: init(){
  sensor.begin();
}

sBmx160SensorData* BMX :: getGyroData(){
  return sensor.getAllData(NULL, &Ogyro, NULL);
}

sBmx160SensorData* BMX :: getAccelData(){
  return sensor.getAllData(NULL, NULL, &OAccel);
}

