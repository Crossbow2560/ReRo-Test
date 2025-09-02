
#include <Arduino.h>
#include <Wire.h>
#include <BMX160.h>
#include <VL6180X.h>

// Example configuration for TOF sensors
const int cePins[] = {2, 3, 6, 7, 10}; // Chip enable pins for TOF sensors
const uint8_t addrs[] = {0x30, 0x31, 0x32, 0x33, 0x34}; // I2C addresses for TOF sensors
const size_t sensorCount = 5;

BMX imu;
TOF tof(cePins, addrs, sensorCount);

void setup() {
	Serial.begin(9600);
	Wire.begin();
	imu.init();
	Serial.println("BMX160 initialized.");
	tof.begin();
	Serial.println("VL6180X TOF initialized.");
}

void loop() {
	// BMX160 test
	sBmx160SensorData_t* gyro = imu.getGyroData();
	sBmx160SensorData_t* accel = imu.getAccelData();
	Serial.print("Gyro: ");
	Serial.print(gyro->x); Serial.print(", ");
	Serial.print(gyro->y); Serial.print(", ");
	Serial.println(gyro->z);
	Serial.print("Accel: ");
	Serial.print(accel->x); Serial.print(", ");
	Serial.print(accel->y); Serial.print(", ");
	Serial.println(accel->z);

	// VL6180X test
	for (size_t i = 0; i < sensorCount; ++i) {
		uint8_t distance = tof.readDistance(i);
		Serial.print("TOF "); Serial.print(i); Serial.print(" distance: ");
		Serial.print(distance); Serial.println(" mm");
	}

	delay(1000);
}
