#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_VL6180X.h>
#include <DFRobot_BMX160.h>

#define N 3
#define N1 2
#define read_delay 100

// Function prototypes
void init_TOF_bus1(int i);
void init_TOF_bus2(int i);
void imu_printdata();

int cePins_bus1[N] = {2,3,6};
int cePins_bus2[N1] = {7,10};
uint8_t addrs_bus1[N] = {0x30,0x31,0x32};
uint8_t addrs_bus2[N1] = {0x33,0x34};
// I2C bus #1 on pins SDA=34, SCL=38
TwoWire I2Cbus1 = TwoWire(1);
// I2C bus #2 on pins SDA=47, SCL=48
TwoWire I2Cbus2 = TwoWire(0);

// TOF Objects
DFRobot_VL6180X tof_bus1[N] = {
  DFRobot_VL6180X(0x29, &I2Cbus1),
  DFRobot_VL6180X(0x29, &I2Cbus1),
  DFRobot_VL6180X(0x29, &I2Cbus1)
};

DFRobot_VL6180X tof_bus2[N1] = {
  DFRobot_VL6180X(0x29, &I2Cbus2),
  DFRobot_VL6180X(0x29, &I2Cbus2)
};

//BMX object
DFRobot_BMX160 BMX_bus2(&I2Cbus2);

void setup(){
  Serial.begin(115200);
  // Initialize both I2C buses
  I2Cbus1.begin(34, 38);   // SDA=34, SCL=38
  I2Cbus2.begin(47, 48);   // SDA=47, SCL=48
  if (BMX_bus2.begin() != true){
    Serial.println("Error initializing BMX160");
    while(1);
  }


  for(int i=0;i<N;i++){ pinMode(cePins_bus1[i], OUTPUT); digitalWrite(cePins_bus1[i], LOW); }
  delay(10);
  for(int i=0;i<N1;i++){ pinMode(cePins_bus2[i], OUTPUT); digitalWrite(cePins_bus2[i], LOW); }
  delay(10);

  for(int i=0;i<N;i++) init_TOF_bus1(i);
  for(int i=0;i<N1;i++) init_TOF_bus2(i);

}

void loop(){
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  for(int i=0;i<N;i++){
    uint8_t mm = tof_bus1[i].rangePollMeasurement();
    Serial.printf("TOF%d (0x%02X): %u mm\n", i, addrs_bus1[i], mm);
    delay(read_delay);
  }
  for(int i=0;i<N1;i++){
    uint8_t mm = tof_bus2[i].rangePollMeasurement();
    Serial.printf("TOF%d (0x%02X): %u mm\n", 3+i, addrs_bus1[i], mm);
    delay(read_delay);
  }
  imu_printdata();
  
}

// ======================= TOF/VL6180X ======================
void init_TOF_bus1(int i){
  digitalWrite(cePins_bus1[i], HIGH);
  delay(10);
  while(!tof_bus1[i].begin()){ 
    Serial.printf("Error Initializing TOF %d \n",i);
  }
  tof_bus1[i].setIICAddr(addrs_bus1[i]);
  delay(50);
}

void init_TOF_bus2(int i){
  digitalWrite(cePins_bus2[i], HIGH);
  delay(10);
  while(!tof_bus2[i].begin()){ 
    Serial.printf("Error Initializing TOF %d \n",i);
  }
  tof_bus2[i].setIICAddr(addrs_bus2[i]);
  delay(50);
}

// ====================== IMU/BMX160 ========================
void imu_printdata()
{
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;

  /* Get a new sensor event */
  BMX_bus2.getAllData(&Omagn, &Ogyro, &Oaccel);

  /* Display the magnetometer results (magn is magnetometer in uTesla) */
  Serial.print("M ");
  Serial.print("X: "); Serial.print(Omagn.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Omagn.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Omagn.z); Serial.print("  ");
  Serial.println("uT");

  /* Display the gyroscope results (gyroscope data is in g) */
  Serial.print("G ");
  Serial.print("X: "); Serial.print(Ogyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Ogyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Ogyro.z); Serial.print("  ");
  Serial.println("g");
  
  /* Display the accelerometer results (accelerometer data is in m/s^2) */
  Serial.print("A ");
  Serial.print("X: "); Serial.print(Oaccel.x    ); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Oaccel.y    ); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Oaccel.z    ); Serial.print("  ");
  Serial.println("m/s^2");

  Serial.println("");

  delay(500);
}