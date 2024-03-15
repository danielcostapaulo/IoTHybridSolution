#include "LSM6DSOX.h"

#define button_pin 2

void setup() {
  Serial.begin(9600);
  while(!Serial); //wait untill Serial is available
  pinMode(button_pin,INPUT_PULLUP);
  if(!IMU_LSM6DSOX.begin()){
    Serial.print("Failed to initialize IMU");
    while(1);
  }
  IMU_LSM6DSOX.init(1);
}

void loop() {
  float x,y,z;
  int button_state=digitalRead(button_pin);
  if(button_state==0){
    Serial.println('c');
    delay(300);
  }
  if(IMU.accelerationAvailable()){
    IMU.readAcceleration(x,y,z);
    x=x*1000;
    y=y*1000;
    z=z*1000;
    Serial.print(x);
    Serial.print("    ");
    Serial.print(y);
    Serial.print("    ");
    Serial.print(z);
    Serial.print("    ");
    Serial.println(sqrt(x*x+y*y+z*z));
  }

}
