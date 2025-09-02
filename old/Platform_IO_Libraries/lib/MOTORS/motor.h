#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class motor{
  private:
    int speed;
    int pinA, pinB, enA, enB;
  public:
    motor(int A, int B, int eA, int eB);
    int setMotorSpeed(int direction, int speed);
    int getMotorSpeed();
};

extern motor right;
extern motor left;

#endif