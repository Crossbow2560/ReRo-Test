#include "ir.h"

IR :: IR(int s, int* pins){
  size = s;
  pinArray = pins;
  for(int i = 0; i < size; i++){
    pinMode(pinArray[i], INPUT);
  }
}

int* IR :: getAll(){
  int* valArray[size];
  for(int i = 0; i < N; i++){
    valArray[i] = digitalRead(pinArray[i]);
  }
  return valArray;
}

int IR :: getVal(int index){
  return digitalRead(pinArray[index]);
}
