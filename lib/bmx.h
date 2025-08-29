#ifndef BMX_H
#define BMX_H

#include <Arduino.h>
#include <DFRobot_BMX160.h>

class BMX{
  private:
    DFRobot_BMX160 sensor;
    sBmx160SensorData_t Ogyro, Oaccel;
  public:
    BMX();
    void init();
    sBmx160SensorData_t* getGyroData();
    sBmx160SensorData_t* getAccelData();
};

#endif
