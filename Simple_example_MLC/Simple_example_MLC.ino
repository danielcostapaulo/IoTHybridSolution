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
#include "Models\FV3.h"
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
#define button_pin 3


//Interrupts.
volatile int mems_event = 0;
float x,y,z;
int GT=0; //Ground truth, 0 is idle, 1 is shake
// Components
int uh;
void INT1Event_cb();
void printMLCStatus(uint8_t status);
int state=0;

void setup() {
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_pin,INPUT);

  // Force INT1 of LSM6DSOX low in order to enable I2C
  pinMode(INT_1, OUTPUT);

  digitalWrite(INT_1, LOW);

  // Initialize serial for output.
  Serial.begin(115200);
  
  // Initialize I2C bus.

  if(!IMU_LSM6DSOX.begin()){
    while(1){
      Serial.println("Could not initialize LSM6DSOX");
    }
  }
  Serial.println("Going to load program now");
  int ProgramSize=sizeof(FV3)/sizeof(ucf_line_t);
  /* Feed the program to Machine Learning Core */
  /* Activity Recognition Default program */  
  if(!IMU_LSM6DSOX.Load_MLC(FV3, ProgramSize)){
    while(1){
      Serial.println("Couldent Load MLC program");
    }
  }
  
  Serial.println("Program loaded inside the LSM6DSOX MLC");

  //Interrupts.
  pinMode(INT_1, INPUT);
  attachInterrupt(INT_1, INT1Event_cb, RISING);


}

void loop() {
    //get IMU.MLC event if there is a change
  if (mems_event) {
    int output=IMU_LSM6DSOX.Get_MLC_Output();
    printMLCStatus(output);
    mems_event=0;
  }
  int button_state=digitalRead(button_pin);
  if (digitalRead(button_pin)==HIGH) GT=1;
  else GT=0;

  //print IMU result when available
  if(IMU_LSM6DSOX.accelerationAvailable()){
    IMU_LSM6DSOX.readAcceleration(x,y,z);
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
      state=0;
      break;
    case 1:
      state=1;
      break;
    default:
      Serial.println("ERROR");
      break;
  }	  
}
