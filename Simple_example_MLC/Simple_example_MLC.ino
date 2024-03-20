/**
 ******************************************************************************
 * @file    X_NUCLEO_IKS01A3_LSM6DSOX_MLC.ino
 * @author  SRA
 * @version V1.1.0
 * @date    March 2020
 * @brief   Arduino test application for the STMicrolectronics X-NUCLEO-IKS01A3
 *          MEMS Inertial and Environmental sensor expansion board.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2020 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
//NOTE: This example isn't compatible with Arduino Uno.
//NOTE: For this example you need the STEVAL-MKI197V1 board connected to the DIL24 connector of the X-NUCLEO-IKS01A3

// Includes
#include "LSM6DSOXSensor.h"
#include "filter.h"
#include "LSM6DSOX.h"

#ifdef ARDUINO_SAM_DUE
#define DEV_I2C Wire1
#elif defined(ARDUINO_ARCH_STM32)
#define DEV_I2C Wire
#elif defined(ARDUINO_ARCH_AVR)
#define DEV_I2C Wire
#else
#define DEV_I2C Wire
#endif
#define SerialPort Serial

#define INT_1 INT_IMU
#define button_pin 2

//Interrupts.
volatile int mems_event = 0;
float x,y,z;
int GT=0; //Ground truth, 0 is idle, 1 is shake
// Components
LSM6DSOXSensor AccGyr(&DEV_I2C, LSM6DSOX_I2C_ADD_L);

// MLC
ucf_line_t *ProgramPointer;
int32_t LineCounter;
int32_t TotalNumberOfLine;

void INT1Event_cb();
void printMLCStatus(uint8_t status);
int state=0;

void setup() {
  uint8_t mlc_out[8];
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_pin,INPUT_PULLUP);

  // Force INT1 of LSM6DSOX low in order to enable I2C
  pinMode(INT_1, OUTPUT);

  digitalWrite(INT_1, LOW);

  delay(200);

  // Initialize serial for output.
  SerialPort.begin(115200);
  
  // Initialize I2C bus.
  DEV_I2C.begin();

  AccGyr.begin();
  IMU_LSM6DSOX.begin();
  IMU_LSM6DSOX.init(1);

  /* Feed the program to Machine Learning Core */
  /* Activity Recognition Default program */  
  ProgramPointer = (ucf_line_t *)filter;
  TotalNumberOfLine = sizeof(filter) / sizeof(ucf_line_t);
  SerialPort.println("Activity Recognition for LSM6DSOX MLC");
  SerialPort.print("UCF Number Line=");
  SerialPort.println(TotalNumberOfLine);

  for (LineCounter=0; LineCounter<TotalNumberOfLine; LineCounter++) {
    if(AccGyr.Write_Reg(ProgramPointer[LineCounter].address, ProgramPointer[LineCounter].data)) {
      SerialPort.print("Error loading the Program to LSM6DSOX at line: ");
      SerialPort.println(LineCounter);
      while(1) {
        // Led blinking.
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
      }
    }
  }

  SerialPort.println("Program loaded inside the LSM6DSOX MLC");

  //Interrupts.
  pinMode(INT_1, INPUT);
  attachInterrupt(INT_1, INT1Event_cb, RISING);

  /* We need to wait for a time window before having the first MLC status */
  delay(3000);

  AccGyr.Get_MLC_Output(mlc_out);
  printMLCStatus(mlc_out[0]);
}

void loop() {
    //get IMU.MLC event if there is a change
    if (mems_event) {
    mems_event=0;
    LSM6DSOX_MLC_Status_t status;
    AccGyr.Get_MLC_Status(&status);
    if (status.is_mlc1) {
      uint8_t mlc_out[8];
      AccGyr.Get_MLC_Output(mlc_out);
      printMLCStatus(mlc_out[0]);
    }
  }
  int button_state=digitalRead(button_pin);
  if(button_state==0){
    if(GT==0) GT=1;
    else GT=0;
    delay(300);
  }

  //print IMU result when available
  if(IMU.accelerationAvailable()){
    IMU.readAcceleration(x,y,z);
    x=x*500;
    y=y*500;
    z=z*500;
    Serial.print(x);
    Serial.print("    ");
    Serial.print(y);
    Serial.print("    ");
    Serial.print(z);
    Serial.print("    ");
    Serial.print(state);
    Serial.print("    ");
    Serial.println(GT);
  }
}

void INT1Event_cb() {
  mems_event = 1;
}

void printMLCStatus(uint8_t status) {
  switch(status) {
    case 0:
      //SerialPort.println("Activity: Idle");
      state=0;
      break;
    case 1:
      //SerialPort.println("Activity: Shaking");
      state=1;
      break;
    default:
      SerialPort.println("ERROR");
      break;
  }	  
}
