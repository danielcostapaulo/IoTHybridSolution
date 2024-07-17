/*
  This file is part of the Arduino_LSM6DSOX library.
  Copyright (c) 2021 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "LSM6DSOX.h"

#define LSM6DSOX_ADDRESS            0x6A

#define LSM6DSOX_WHO_AM_I_REG       0X0F
#define LSM6DSOX_CTRL1_XL           0X10
#define LSM6DSOX_CTRL2_G            0X11

#define LSM6DSOX_STATUS_REG         0X1E

#define LSM6DSOX_CTRL6_C            0X15
#define LSM6DSOX_CTRL7_G            0X16
#define LSM6DSOX_CTRL8_XL           0X17

#define LSM6DSOX_OUT_TEMP_L         0X20
#define LSM6DSOX_OUT_TEMP_H         0X21

#define LSM6DSOX_OUTX_L_G           0X22
#define LSM6DSOX_OUTX_H_G           0X23
#define LSM6DSOX_OUTY_L_G           0X24
#define LSM6DSOX_OUTY_H_G           0X25
#define LSM6DSOX_OUTZ_L_G           0X26
#define LSM6DSOX_OUTZ_H_G           0X27

#define LSM6DSOX_OUTX_L_XL          0X28
#define LSM6DSOX_OUTX_H_XL          0X29
#define LSM6DSOX_OUTY_L_XL          0X2A
#define LSM6DSOX_OUTY_H_XL          0X2B
#define LSM6DSOX_OUTZ_L_XL          0X2C
#define LSM6DSOX_OUTZ_H_XL          0X2D

#define EMB_FUNC_EN_A               0x04
#define EMB_FUNC_EN_B               0x05
#define EMB_FUNC_INT1               0x0A
#define EMB_FUNC_STATUS             0x12
#define EMB_FUNC_INIT_A             0x66
#define FUNC_CFG_ACCESS             0x01
#define MD1_CFG                     0x5E
#define PAGE_RW                     0x17
#define TAP_CFG0                    0x56
#define TAP_CFG2                    0x58
#define WAKE_UP_DUR                 0x5C
#define WAKE_UP_THS                 0x5B

#define MLC0_SRC                    0x70
#define MLC_STATUS                  0x15

#define FIFO_CTRL1                  0x07
#define FIFO_CTRL2                  0x08
#define FIFO_CTRL3                  0x09
#define FIFO_CTRL4                  0x0A
#define INT1_CTRL                   0x0D
#define INT2_CTRL                   0x0E
#define FIFO_STATUS1                0x3A
#define FIFO_STATUS2                0x3B
#define FIFO_DATA_OUT_X_L           0x79 



LSM6DSOXClass::LSM6DSOXClass(TwoWire& wire, uint8_t slaveAddress) :
  _wire(&wire),
  _spi(NULL),
  _slaveAddress(slaveAddress)
{
}

LSM6DSOXClass::LSM6DSOXClass(SPIClass& spi, int csPin) :
  _wire(NULL),
  _spi(&spi),
  _csPin(csPin),
  _spiSettings(10E6, MSBFIRST, SPI_MODE0)
{
}

LSM6DSOXClass::~LSM6DSOXClass()
{
}

int LSM6DSOXClass::begin()
{
  if (_spi != NULL) {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    _spi->begin(12,13,11,10);
  } else {
    _wire->begin();
  }
  readRegister(LSM6DSOX_WHO_AM_I_REG);
  if (!(readRegister(LSM6DSOX_WHO_AM_I_REG) == 0x6C || readRegister(LSM6DSOX_WHO_AM_I_REG) == 0x69)) {

    return readRegister(LSM6DSOX_WHO_AM_I_REG);
  }



  return 1;
}

/*
To initialize, its necessasry to use the function bellow in which level defines
the energy level of the accelerometer in the following way:
1-low power-> 26Hz,4g 
2-medium power-> 1.66kHz,8g 
3-high power-> 6,6kHz,8g

The logic of these values are for 1 to be used when only using the accelerometer MLC, 2 for state prediction and 3 for gathering data.
*/
int LSM6DSOXClass::init(int level) 
{
  int val;
  _level=level;
  if(level==1) {
    val=0x28;
    _g=4;
  }
  else if(level==2){
    val=0x8C;
    _g=8;
  }
  else{
    val=0xAC;
    _g=8;
  }  
  if(!writeRegister(LSM6DSOX_CTRL1_XL, val)){
    return 0;
  }

  if(!writeRegister(LSM6DSOX_CTRL6_C,0x10)){ //this register will remove the accelerometer from high performance mode
    return 0;
  }
  return 1;
}

void LSM6DSOXClass::end()
{
  if (_spi != NULL) {
    _spi->end();
    digitalWrite(_csPin, LOW);
    pinMode(_csPin, INPUT);
  } else {
    writeRegister(LSM6DSOX_CTRL2_G, 0x00);
    writeRegister(LSM6DSOX_CTRL1_XL, 0x00);
    _wire->end();
  }
}


int LSM6DSOXClass::readAcceleration(float& x, float& y, float& z)
{
  int16_t data[3];

  if (!readRegisters(LSM6DSOX_OUTX_L_XL, (uint8_t*)data, sizeof(data))) {
    x = NAN;
    y = NAN;
    z = NAN;

    return 0;
  }

  x = data[0] * _g / 32.768;
  y = data[1] * _g / 32.768;
  z = data[2] * _g / 32.768;

  return 1;
}

int LSM6DSOXClass::accelerationAvailable()
{
  if (readRegister(LSM6DSOX_STATUS_REG) & 0x01) {
    return 1;
  }

  return 0;
}

float LSM6DSOXClass::accelerationSampleRate()
{
  return 104.0F;
}

int LSM6DSOXClass::readGyroscope(float& x, float& y, float& z)
{
  int16_t data[3];

  if (!readRegisters(LSM6DSOX_OUTX_L_G, (uint8_t*)data, sizeof(data))) {
    x = NAN;
    y = NAN;
    z = NAN;

    return 0;
  }

  x = data[0] * 2000.0 / 32768.0;
  y = data[1] * 2000.0 / 32768.0;
  z = data[2] * 2000.0 / 32768.0;

  return 1;
}

int LSM6DSOXClass::gyroscopeAvailable()
{
  if (readRegister(LSM6DSOX_STATUS_REG) & 0x02) {
    return 1;
  }

  return 0;
}

int LSM6DSOXClass::readTemperature(int& temperature_deg)
{
  float temperature_float = 0;
  readTemperatureFloat(temperature_float);

  temperature_deg = static_cast<int>(temperature_float);

  return 1;
}

int LSM6DSOXClass::readTemperatureFloat(float& temperature_deg)
{
  /* Read the raw temperature from the sensor. */
  int16_t temperature_raw = 0;

  if (readRegisters(LSM6DSOX_OUT_TEMP_L, reinterpret_cast<uint8_t*>(&temperature_raw), sizeof(temperature_raw)) != 1) {
    return 0;
  }

  /* Convert to °C. */
  static int const TEMPERATURE_LSB_per_DEG = 256;
  static int const TEMPERATURE_OFFSET_DEG = 25;

  temperature_deg = (static_cast<float>(temperature_raw) / TEMPERATURE_LSB_per_DEG) + TEMPERATURE_OFFSET_DEG;

  return 1;
}

int LSM6DSOXClass::temperatureAvailable()
{
  if (readRegister(LSM6DSOX_STATUS_REG) & 0x04) {
    return 1;
  }

  return 0;
}

void LSM6DSOXClass::enableSMD()
{
  
  writeRegister(FUNC_CFG_ACCESS,0x80);
  writeRegister(EMB_FUNC_EN_A,0x20);
  writeRegister(EMB_FUNC_INT1,0x20);
  writeRegister(FUNC_CFG_ACCESS,0x00);
  writeRegister(MD1_CFG, 0x02);
}

int LSM6DSOXClass::readSMD()
{
  writeRegister(FUNC_CFG_ACCESS,0x80);
  int val=readRegister(EMB_FUNC_STATUS);
  writeRegister(FUNC_CFG_ACCESS,0x00);
  return val;
}

void LSM6DSOXClass::enableWakeUp()
{
  writeRegister(TAP_CFG0, 0x51);
  writeRegister(TAP_CFG2,0x80);
  writeRegister(WAKE_UP_DUR,0x00);
  writeRegister(WAKE_UP_THS,0x02);
  writeRegister(MD1_CFG,0x20);
}

float LSM6DSOXClass::gyroscopeSampleRate()
{
  return 104.0F;

}


//This automaticaly enables the MLC and does all of the initialization thanks to UNICO-GUI. More custimization might be required.
int LSM6DSOXClass::Load_MLC(const ucf_line_t MLC[],int size){ 
  ucf_line_t *ProgramPointer;
  int32_t LineCounter;
  ProgramPointer = (ucf_line_t *) MLC;
  for (LineCounter=0; LineCounter<size; LineCounter++) {
    if(!writeRegister(ProgramPointer[LineCounter].address, ProgramPointer[LineCounter].data)) {
      return -1;
    }
  }
  return 1;
}

int LSM6DSOXClass::Get_MLC_Status(){
  writeRegister(FUNC_CFG_ACCESS,0x80);
  int val=readRegister(MLC_STATUS);
  writeRegister(FUNC_CFG_ACCESS,0x00);
  return val;
}
int LSM6DSOXClass::Get_MLC_Output(){ 
  writeRegister(FUNC_CFG_ACCESS,0x80);
  int val=readRegister(MLC0_SRC);
  writeRegister(FUNC_CFG_ACCESS,0x00);
  return val;
}

/*
To use the fifo, its neccessary to define its batch rate which is in the following way:
1-low power-> 26Hz
2-medium power-> 208Hz
3-high power-> 6,6kHz

*/
int LSM6DSOXClass::setFIFOBRT(int level){
  int val=0;
  if(level==1)val=0x02;
  else if(level==2)val=0x05;
  else val=0x0A;
  if(!writeRegister(FIFO_CTRL3,val)) return 0;
  return 1;
}


/*
The function bellow selects the desired mode:

0->bypass mode (FIFO clears and is disabled)
1->Continous mode (FIFO writes on top of old samples)

*/
int LSM6DSOXClass::setFIFOMode(int mode){
  int val=0;
  if (mode==0) val=0x00;
  else if(mode==1) val=0x06;
  if(!writeRegister(FIFO_CTRL4,val)) return 0;
  return 1;
}
/*
The function bellow selects the interrupt pin (1 or 2) that will be activated when the waterlevel threshold is reached:

1-> INT1
2-> INT2

*/
int LSM6DSOXClass::setFIFOInterrupt(int pin){

  if (pin==1) {
    if(!writeRegister(INT1_CTRL,0x20)) return 0;
    return 1;
  }
  else{
    if(!writeRegister(INT2_CTRL,0x04)) return 0;
    return 1;
  } 
}

/*
The function bellow defines the waterlevel threshold value:

1->26
2->130
3->255

*/
int LSM6DSOXClass::setFIFOWT(int wtl){
  int val=0;
  if(wtl==1) val=0x1A;
  else if(wtl==2) val=0x82;
  else val=0xFF;
  if(!writeRegister(FIFO_CTRL1,val)){
    return 0;
  }
  if(!writeRegister(FIFO_CTRL2,0x00)){
    return 0;
  }
  return 1;

}

int LSM6DSOXClass::setFIFOlimit(int lim){
  if(!writeRegister(FIFO_CTRL1,0xFF)) return 0;
  if(!writeRegister(FIFO_CTRL2,0x81)) return 0;
  return 1;
}



int LSM6DSOXClass::getFIFOSamples(){
  int n_samples1=0;
  n_samples1=readRegister(FIFO_STATUS1);
  int n_samples2=readRegister(FIFO_STATUS2);
    if(n_samples1==-1 || n_samples2==-1){
    return -1;
  }
  int mask=0b00000011;
  n_samples2=n_samples2&mask;
  int n_samples=(n_samples2<<8)|n_samples1;

  return n_samples;
}

int LSM6DSOXClass::getFIFOAcc(float& x,float& y, float& z){
  int16_t data[3];
  if (!readRegisters(FIFO_DATA_OUT_X_L, (uint8_t*)data, sizeof(data))) {
    x = NAN;
    y = NAN;
    z = NAN;

    return 0;
  }
  x = data[0] * _g / 32.768;
  y = data[1] * _g / 32.768;
  z = data[2] * _g / 32.768;

  return 1;
}

int LSM6DSOXClass::readRegister(uint8_t address)
{
  uint8_t value;

  if (readRegisters(address, &value, sizeof(value)) != 1) {
    return -1;
  }

  return value;
}

int LSM6DSOXClass::readRegisters(uint8_t address, uint8_t* data, size_t length)
{
  uint8_t test_val;
  if (_spi != NULL) {
    _spi->beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    _spi->transfer(0x80 | address);
    for(int i=0;i<length;i++){
      data[i]=_spi->transfer(0x00);
    }
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
    
  } else {
    _wire->beginTransmission(_slaveAddress);
    _wire->write(address);

    if (_wire->endTransmission(false) != 0) {
      return -1;
    }

    if (_wire->requestFrom(_slaveAddress, length) != length) {
      return 0;
    }

    for (size_t i = 0; i < length; i++) {
      *data++ = _wire->read();
    }
  }
  return 1;
}

int LSM6DSOXClass::writeRegister(uint8_t address, uint8_t value)
{
  if (_spi != NULL) {
    _spi->beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    _spi->transfer(address);
    _spi->transfer(value);
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
  } else {
    _wire->beginTransmission(_slaveAddress);
    _wire->write(address);
    _wire->write(value);
    if (_wire->endTransmission() != 0) {
      return 0;
    }
  }
  return 1;
}
