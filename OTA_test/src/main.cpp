#include "OTA.h"

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  Serial.print("im the second firmware");

  connectToWiFi();

}

void loop() {
  if(checkFileFromServer()){
    performOTAUpdateFromSPIFFS();
  }
  else{
    delay(5000);
  }
}

