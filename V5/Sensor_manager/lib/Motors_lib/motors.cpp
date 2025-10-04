#include "motors.h"

void motor::motors_init(){
    // motor driver pins
    pinMode(_leftMotorPinA, OUTPUT);
    pinMode(_leftMotorPinB, OUTPUT);
    pinMode(_rightMotorPinA, OUTPUT);
    pinMode(_rightMotorPinB, OUTPUT);

    // rotary encoders
    pinMode(_leftEnA, INPUT);
    pinMode(_leftEnB, INPUT);
    pinMode(_rightEnA, INPUT);
    pinMode(_rightEnB, INPUT);
}

void motor::setLeftSpeed(int direction, int speed){
  // clamp to max or min values if exceeded
  if(speed < 0){speed = 0;}
  if(direction > 0){
    if(speed > _forward_threshold){speed = _forward_threshold;}
  }
  else if(direction < 0){
    if(speed > _backward_threshold){speed = _backward_threshold;}
  }

  if(direction > 0){
    analogWrite(_leftMotorPinA, speed);
    analogWrite(_leftMotorPinB, 0);
  }
  else if(direction < 0){
    analogWrite(_leftMotorPinA, 0);
    analogWrite(_leftMotorPinB, speed);
  }
  else{
    direction = 0;
    analogWrite(_leftMotorPinA, 0);
    analogWrite(_leftMotorPinB, 0);
  }
}

void motor::setRightSpeed(int direction, int speed){
  // clamp to max or min values if exceeded
  if(speed < 0){speed = 0;}
  if(direction > 0){
    if(speed > _forward_threshold){speed = _forward_threshold;}
  }
  else if(direction < 0){
    if(speed > _backward_threshold){speed = _backward_threshold;}
  }

  if(direction > 0){
    analogWrite(_rightMotorPinA, speed);
    analogWrite(_rightMotorPinB, 0);
  }
  else if(direction < 0){
    analogWrite(_rightMotorPinA, 0);
    analogWrite(_rightMotorPinB, speed);
  }
  else{
    direction = 0;
    analogWrite(_rightMotorPinA, 0);
    analogWrite(_rightMotorPinB, 0);
  }
}