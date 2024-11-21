#include "LSM6DSOX.h"
#include <SPI.h>

#define button_pin 2

SPIClass dev_spi;
LSM6DSOXClass Acc(dev_spi,SS);

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Initialize I2C bus.
  int test=Acc.begin();
  if(test!=1){
    Serial.println(test);
    Serial.println("Could not initialize LSM6DSOX");
    Acc.end();
    while(1);
  }
  Acc.init(1);
}

void loop() {
  float x,y,z;
  int button_state=digitalRead(button_pin);
  /*
  if(button_state==0){
    Serial.println('c');
    delay(300);
  }
  */
  if(Acc.accelerationAvailable()){
    Acc.readAcceleration(x,y,z);
    Serial.print(x);
    Serial.print("    ");
    Serial.print(y);
    Serial.print("    ");
    Serial.print(z);
    Serial.print("    ");
    Serial.println(sqrt(x*x+y*y+z*z));
  }

}
