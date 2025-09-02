#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>

#define N 2
int cePins[N] = {2,3};
uint8_t addrs[N] = {0x30,0x31};

DFRobot_VL6180X tof[N];

void init_TOF(int i){
  digitalWrite(cePins[i], HIGH);
  delay(10);
  while(!tof[i].begin()){ 
    Serial.printf("Error Initializing TOF %d \n",i);
  }
  tof[i].setIICAddr(addrs[i]);
  delay(50);
}

void setup(){
  Serial.begin(115200);
  Wire.begin();

  for(int i=0;i<N;i++){ pinMode(cePins[i], OUTPUT); digitalWrite(cePins[i], LOW); }
  delay(10);

  for(int i=0;i<N;i++) init_TOF(i);
}

void loop(){
  for(int i=0;i<N;i++){
    
    uint8_t mm = tof[i].rangePollMeasurement();
    Serial.printf("TOF%d (0x%02X): %u mm\n", i, addrs[i], mm);
    delay(50);
  }
  
}







