#include "SensorManager.h"
#include <Arduino.h>

// ---------- Global variables and Instances ----------
// TwoWire instances constructor initialization
TwoWire SensorManager::I2Cbus1_{1};
TwoWire SensorManager::I2Cbus2_{0};

// CE pins per bus for TOF sensors
const int SensorManager::cePinsBus1_[3] = {2, 3, 6};
const int SensorManager::cePinsBus2_[2] = {7, 10};

// I2C addresses per bus for TOF sensors
const uint8_t SensorManager::iicAddrsBus1_[3] = {0x30, 0x31, 0x32};
const uint8_t SensorManager::iicAddrsBus2_[2] = {0x33, 0x34};

// Flattened CE Pins arrays (bus1 + bus2) for TOF sensors
const int SensorManager::cePins_[TOF_COUNT]   = {2, 3, 6, 7, 10};
const uint8_t SensorManager::iicAddrs_[TOF_COUNT] = {0x30, 0x31, 0x32, 0x33, 0x34};

// Bus assignment map for TOF sensors
TwoWire* SensorManager::busMapTOF_[TOF_COUNT] = {
    &I2Cbus1_, &I2Cbus1_, &I2Cbus1_,
    &I2Cbus2_, &I2Cbus2_
};

// Bus assignment map for AS7431 sensors
TwoWire* SensorManager::busMapAS7341_[AS7341_COUNT] = {
    &I2Cbus1_, &I2Cbus2_
};

//---------- Sensor objects ----------
DFRobot_VL6180X* SensorManager::sensors_[TOF_COUNT];    // TOF Sensor object
DFRobot_BMX160* SensorManager::bmx_;                    // BMX Sensor object
QTRSensors SensorManager::qtr_;                         // IR Array Sensor object
DFRobot_AS7341* SensorManager::as7341_[AS7341_COUNT];   // AS7431 Colour Sensor object

//---------- Double buffers ----------
// TOF Double buffers
SensorManager::TOFData SensorManager::bufferTOFA_;
SensorManager::TOFData SensorManager::bufferTOFB_;
std::atomic<SensorManager::TOFData*> SensorManager::activeTOFBuffer_{&SensorManager::bufferTOFA_};
// BMX Double buffers
SensorManager::IMUData SensorManager::bufferBMXA_;
SensorManager::IMUData SensorManager::bufferBMXB_;
std::atomic<SensorManager::IMUData*> SensorManager::activeBMXBuffer_{&SensorManager::bufferBMXA_};
// IR Double buffers
SensorManager::IRData SensorManager::bufferIRA_;
SensorManager::IRData SensorManager::bufferIRB_;
std::atomic<SensorManager::IRData*> SensorManager::activeIRBuffer_{&SensorManager::bufferIRA_};
// AS7341 Double buffers
SensorManager::AS7341Data SensorManager::bufferAS7341_1A_;
SensorManager::AS7341Data SensorManager::bufferAS7341_1B_;
SensorManager::AS7341Data SensorManager::bufferAS7341_2A_;
SensorManager::AS7341Data SensorManager::bufferAS7341_2B_;
std::atomic<SensorManager::AS7341Data*> SensorManager::activeAS7341_1Buffer_{&SensorManager::bufferAS7341_1A_};
std::atomic<SensorManager::AS7341Data*> SensorManager::activeAS7341_2Buffer_{&SensorManager::bufferAS7341_2A_};

// ---------- Private Methods ----------
// Initialize all sensors
void SensorManager::init_I2C(){
    // Init I2C buses
    I2Cbus1_.begin(34, 38);   // SDA=34, SCL=38
    I2Cbus2_.begin(8,9);      // SDA=8, SCL=9
}

void SensorManager::init_TOF(){
    // Reset all CE pins of TOF to low
    for (size_t i = 0; i < TOF_COUNT-1; i++) {
        pinMode(cePins_[i], OUTPUT);
        digitalWrite(cePins_[i], LOW);
    }
    delay(10);

    // Initialize TOF sensors
    for (size_t i = 0; i < TOF_COUNT-1; i++) {
        sensors_[i] = new DFRobot_VL6180X(0x29, busMapTOF_[i]);
        digitalWrite(cePins_[i], HIGH);
        delay(100);
        // Check if TOF has been intiialized 
        while(!sensors_[i]->begin()) {
            Serial.printf("ERROR: TOF %d not initialized\n", i);
            delay(1000);
        }
        delay(100);
        sensors_[i]->setIICAddr(iicAddrs_[i]);
        delay(100);
    }
}

void SensorManager::init_BMX(){
    // Initialize BMX160 on bus2
    bmx_ = new DFRobot_BMX160(&I2Cbus2_);
    if (!bmx_->begin()) {
        Serial.println("ERROR: BMX160 not initialized!");
    }
}

void SensorManager::init_IR(){
    // Initialize IR Array
    qtr_.setTypeRC();
    qtr_.setSensorPins((const uint8_t[]){19,40,41,16,15,14,13,12}, IR_COUNT);
    qtr_.setEmitterPin(42);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    qtr_.setEmitterPin(42);
    // Calibrate the IR Array
    for (uint16_t i = 0; i < 40; i++) {
        qtr_.calibrate();
    }
    
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
}

void SensorManager::init_AS7341(){
    // Initialize AS7341
    for(size_t i = 0; i < AS7341_COUNT; i++){
        as7341_[i] = new DFRobot_AS7341(busMapAS7341_[i]);
        delay(10);
        // Check if AS7341 has been initialized
        while(!as7341_[i]->begin()){
            Serial.printf("ERROR: AS7341 %d not initialized\n", i);
            delay(1000);
        }
        as7341_[i]->enableLed(1);
        as7341_[i]->controlLed(LED_BRIGHTNESS);
    }
}

void SensorManager::init_main() {
    init_I2C();
    init_TOF();
    init_BMX();
    init_IR();
    //init_AS7341();
    
}

// ---------- TOF Methods ----------
void SensorManager::updateTOF_() {
    TOFData* writeBuf = (activeTOFBuffer_.load(std::memory_order_relaxed) == &bufferTOFA_) ? &bufferTOFB_ : &bufferTOFA_;
    for (size_t i = 0; i < TOF_COUNT-1; i++) {
        writeBuf->tof[i] = sensors_[i]->rangePollMeasurement();
    }
    activeTOFBuffer_.store(writeBuf, std::memory_order_release);
}

SensorManager::TOFData SensorManager::readTOF_() {
    TOFData snapshot;
    TOFData* src = activeTOFBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(TOFData));
    return snapshot;
}

// ---------- BMX Methods ----------
void SensorManager::updateBMX_() {
    IMUData* writeBuf = (activeBMXBuffer_.load(std::memory_order_relaxed) == &bufferBMXA_) ? &bufferBMXB_ : &bufferBMXA_;
    sBmx160SensorData_t accel, gyro, mag;
    bmx_->getAllData(&mag, &gyro, &accel);
    writeBuf->accel[0] = accel.x; writeBuf->accel[1] = accel.y; writeBuf->accel[2] = accel.z;
    writeBuf->gyro[0]  = gyro.x;  writeBuf->gyro[1]  = gyro.y;  writeBuf->gyro[2]  = gyro.z;
    writeBuf->magneto[0] = mag.x; writeBuf->magneto[1] = mag.y; writeBuf->magneto[2] = mag.z;
    activeBMXBuffer_.store(writeBuf, std::memory_order_release);
}

SensorManager::IMUData SensorManager::readBMX_() {
    IMUData snapshot;
    IMUData* src = activeBMXBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(IMUData));
    return snapshot;
}

// ---------- IR Methods ----------
void SensorManager::updateIR_() {
    IRData* writeBuf = (activeIRBuffer_.load(std::memory_order_relaxed) == &bufferIRA_) ? &bufferIRB_ : &bufferIRA_;

    uint16_t rawVals[IR_COUNT];
    qtr_.read(rawVals); // use read() instead of readCalibrated() for actual RC pulse widths

    for (size_t i = 0; i < IR_COUNT; i++) {
        writeBuf->values[i] = (rawVals[i] > IR_THRESHOLD) ? 1 : 0; // binary 0/1
    }
   
    activeIRBuffer_.store(writeBuf, std::memory_order_release);
}

SensorManager::IRData SensorManager::readIR_() {
    IRData snapshot;
    IRData* src = activeIRBuffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(IRData));
    return snapshot;
}

SensorManager::IRData SensorManager::readIRRaw_() {
    IRData snapshot;
    uint16_t rawVals[IR_COUNT];
    qtr_.read(rawVals);  // get the actual raw pulse widths

    for (size_t i = 0; i < IR_COUNT; i++) {
        snapshot.values[i] = rawVals[i];
    }

    return snapshot;
}

// ---------- AS7341 Methods ----------
void SensorManager::updateAS7341_1_(){
    AS7341Data* writeBuf = 
        (activeAS7341_1Buffer_.load(std::memory_order_relaxed) == &bufferAS7341_1A_)? &bufferAS7341_1B_: &bufferAS7341_1A_;

    // Mode 1: F1-F4
    as7341_[0]->startMeasure(DFRobot_AS7341::eF1F4ClearNIR);
    DFRobot_AS7341::sModeOneData_t data1 = as7341_[0]->readSpectralDataOne();
    writeBuf->f1 = data1.ADF1;
    writeBuf->f2 = data1.ADF2;
    writeBuf->f3 = data1.ADF3;
    writeBuf->f4 = data1.ADF4;
    writeBuf->clear = data1.ADCLEAR;
    writeBuf->nir = data1.ADNIR;

    // Mode 2: F5-F8
    as7341_[0]->startMeasure(DFRobot_AS7341::eF5F8ClearNIR);
    DFRobot_AS7341::sModeTwoData_t data2 = as7341_[0]->readSpectralDataTwo();
    writeBuf->f5 = data2.ADF5;
    writeBuf->f6 = data2.ADF6;
    writeBuf->f7 = data2.ADF7;
    writeBuf->f8 = data2.ADF8;

    activeAS7341_1Buffer_.store(writeBuf, std::memory_order_release);
}

void SensorManager::updateAS7341_2_(){
    AS7341Data* writeBuf = 
        (activeAS7341_2Buffer_.load(std::memory_order_relaxed) == &bufferAS7341_2A_)? &bufferAS7341_2B_: &bufferAS7341_2A_;

    // Mode 1: F1-F4
    as7341_[1]->startMeasure(DFRobot_AS7341::eF1F4ClearNIR);
    DFRobot_AS7341::sModeOneData_t data1 = as7341_[1]->readSpectralDataOne();
    writeBuf->f1 = data1.ADF1;
    writeBuf->f2 = data1.ADF2;
    writeBuf->f3 = data1.ADF3;
    writeBuf->f4 = data1.ADF4;
    writeBuf->clear = data1.ADCLEAR;
    writeBuf->nir = data1.ADNIR;

    // Mode 2: F5-F8
    as7341_[1]->startMeasure(DFRobot_AS7341::eF5F8ClearNIR);
    DFRobot_AS7341::sModeTwoData_t data2 = as7341_[1]->readSpectralDataTwo();
    writeBuf->f5 = data2.ADF5;
    writeBuf->f6 = data2.ADF6;
    writeBuf->f7 = data2.ADF7;
    writeBuf->f8 = data2.ADF8;
    
    activeAS7341_2Buffer_.store(writeBuf, std::memory_order_release);
}

void SensorManager::updateAS7341(){
    updateAS7341_1_();
    updateAS7341_2_();   
}

SensorManager::AS7341Data SensorManager::readAS7341_1(){
    AS7341Data snapshot;
    AS7341Data* src = activeAS7341_1Buffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(AS7341Data));
    return snapshot;
}

SensorManager::AS7341Data SensorManager::readAS7341_2(){
    AS7341Data snapshot;
    AS7341Data* src = activeAS7341_2Buffer_.load(std::memory_order_acquire);
    memcpy(&snapshot, src, sizeof(AS7341Data));
    return snapshot;
}

