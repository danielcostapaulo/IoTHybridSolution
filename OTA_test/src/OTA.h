#include <WiFi.h>
#include <SPIFFS.h>
#include "Update.h"
#include <WiFiClientSecure.h>

// Define WiFi credentials
#define ssid "DaniS24"
#define password "nineplusten"

// Define server details and file path
#define HOST "raw.githubusercontent.com"
#define PATH "/danielpaulinho/IST-vibration/main/Kmeans_solo/OTA_test.bin"
#define PORT 443

// Define the name for the downloaded firmware file
#define FILE_NAME "firmware.bin"

void connectToWiFi();
int  checkFileFromServer();
void performOTAUpdateFromSPIFFS();
