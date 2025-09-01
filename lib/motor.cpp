#include "motor.h"

int threshold = 255;

motor::motor(int A, int B, int eA, int eB)
  :pinA(A), pinB(B), enA(eA), enB(eB){
    speed = 0;
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
}

int motor::setMotorSpeed(int direction, int speed){
  // clamp to max or min values if exceeded
  if(speed > threshold){speed = threshold;}
  else if(speed < 0){speed = 0;}

  if(direction > 0){
    analogWrite(pinA, speed);
    analogWrite(pinB, 0);
  }
  else if(direction < 0){
    analogWrite(pinB, speed);
    analogWrite(pinA, 0);
  }
  else{
    direction = 0;
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  }
  return 0;
}

int motor::getMotorSpeed(){
  return speed;
}

motor right(33, 35, 4, 5);
motor left(36, 37, 8, 11);