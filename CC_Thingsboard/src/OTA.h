#include <WiFi.h>
#include <SPIFFS.h>
#include "Update.h"
#include <WiFiClientSecure.h>

// Define WiFi credentials
#define ssid "SSID"
#define w_password "PASSWORD"

// Define server details and file path
#define HOST "raw.githubusercontent.com"
#define PATH "/danielpaulinho/IoTHybridSolution/main/Firmware/RF_model.bin"
#define PORT 443

// Define the name for the downloaded firmware file
#define FILE_NAME "firmware.bin"

void connectToWiFi();
int  checkFileFromServer();
void performOTAUpdateFromSPIFFS();
