#include <DFRobot_BMX160.h>

DFRobot_BMX160 sensor;

void setup() {
  Serial.begin(115200);
  delay(100);
  while(!sensor.begin()){
    delay(100);
  }
}

void loop() {
  sBmx160SensorData_t Ogyro, Oaccel;
  sensor.getAllData(NULL, &Ogyro, &Oaccel);

  Serial.print("G ");
  Serial.print("X: "); Serial.print(Ogyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Ogyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Ogyro.z); Serial.print("  ");
  Serial.println("g");

  Serial.print("A ");
  Serial.print("X: "); Serial.print(Oaccel.x    ); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Oaccel.y    ); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Oaccel.z    ); Serial.print("  ");
  Serial.println("m/s^2");

  delay(100);
}
