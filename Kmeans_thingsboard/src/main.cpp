#include "LSM6DSOX.h"
#include <SPI.h>
#include "fft_handler.h"
#include "Kmeans.h"
#include "Update.h"
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
#include <WiFi.h>

//Things for Thingsboard
//constexpr char WIFI_SSID[] = "MEO-A600E0";
constexpr char WIFI_SSID[] = "DaniS24";
//constexpr char WIFI_PASSWORD[] = "f1ca76e7f4";
constexpr char WIFI_PASSWORD[] = "nineplusten";
constexpr char TOKEN[] = "DlrFvqezcvvPmS1KqIJO";
constexpr char THINGSBOARD_SERVER[] = "iot.istartlab.tecnico.ulisboa.pt";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr uint32_t MAX_MESSAGE_SIZE = 7800U;
// Initialize underlying client, used to establish a connection
WiFiClient wifiClient;
// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(wifiClient);
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been succesfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
const bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}


#define INT_1 1
#define GT_pin 2

//Objects related to the Accelerometer
SPIClass dev_spi;
LSM6DSOXClass Acc(dev_spi,SS);

//Interrupts.
volatile int mems_event = 0;
float x,y,z;
int GT=0; //Ground truth, 0 is idle, 1 is shake

//reated to the Accelerometer DT
void INT1Event_cb();
void getMLCStatus(uint8_t status);
int state=0;

//FFT
#define SAMPLE_FREQ 6666 //sampling frequency of microphone
#define SAMPLES 256 //nº of samples for FFT
int x_axis[SAMPLES];
int y_axis[SAMPLES];
int z_axis[SAMPLES];
int sample_n=0;

int FFTbuffer[SAMPLES];   // FFT sample buffer
float FFT_amp[SAMPLES];   // FFT output
FFT_handler FFT=FFT_handler(SAMPLES,1, FFTbuffer, FFT_amp);

//Kmeans
int Sample_to_pred[SAMPLES*3];
Kmeans_OT clf;
int kmeans_state=-1;

int* anomaly_to_thingsboard;
char anomaly_str[SAMPLES*3*10+1];

void setup() {
  // Initalize serial connection
  Serial.begin(115200);
  delay(1000);
  
  pinMode(GT_pin, INPUT);
  pinMode(INT_1, OUTPUT);
  digitalWrite(INT_1, LOW);

  InitWiFi();
  
  // Initialize Acc.
  int test=Acc.begin();
  if(test!=1){
    Serial.println(test,HEX);
    Serial.println("Could not initialize LSM6DSOX");
    Acc.end();
    while(1);
  }
  Serial.println("Going to load program now");
  int ProgramSize=sizeof(final)/sizeof(ucf_line_t);

  /* Feed the program to Machine Learning Core */
  if(!Acc.Load_MLC(final, ProgramSize)){
    while(1){
      Serial.println("Couldent Load MLC program");
    }
  }
    Serial.println("Program loaded inside the LSM6DSOX MLC");
  //Force the accelerometer to work in 6666Hz,8g
  Acc.init(3);

  //interrupts for ACC DT
  pinMode(INT_1, INPUT);
  attachInterrupt(INT_1, INT1Event_cb, RISING);
}

void loop() {

  if (!reconnect()) {
    return;
  }
    if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
    // Sending a MAC address as an attribute
    tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());

    Serial.println("Subscribing for RPC...");
    // Perform a subscription. All consequent data processing will happen in
    // processSetLedState() and processSetLedMode() functions,
    // as denoted by callbacks array.

    Serial.println("Subscribe done");
  }

  //checks for ACC DT status
  if (mems_event) {
    int output=Acc.Get_MLC_Output();
    getMLCStatus(output);
    mems_event=0;
  }
  //checks GT for ON/OFF if available
  int button_state=digitalRead(GT_pin);
  if (digitalRead(GT_pin)==HIGH) GT=1;
  else GT=0;

  //get Acc values if available
  if(Acc.accelerationAvailable()){
    Acc.readAcceleration(x,y,z);
    x_axis[sample_n]=x;
    y_axis[sample_n]=y;
    z_axis[sample_n]=z;
    sample_n++;
  }

  //buffers are full and ready for FFT & Kmeans
  if(sample_n==SAMPLES-1){
    //-----------------------------------make FFT of all axis-------------------------
    FFT.Windowing(FFT_WIN_TYPE_HAMMING);
    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=x_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i]=FFT_amp[i];

    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=y_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i+SAMPLES]=FFT_amp[i];
    
    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=z_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i+2*SAMPLES]=FFT_amp[i];
    //--------------------------------------------------------------------------------
    kmeans_state=clf.predict(Sample_to_pred);
    sample_n=0;
    Serial.print("ACC state:");Serial.print(state);Serial.print("; Kmeans state:");Serial.println(kmeans_state);
    //send the anomalies to thingsboard for future use
    if(kmeans_state==-2){
      Serial.print("Sending anomalies to Thingsboard");
      for(int i=0;i<50;i++){
        anomaly_to_thingsboard=clf.get_anomaly(i);
        for(int j=0;j<SAMPLES*3;j++){
          if(j<SAMPLES*3-1) sprintf(&anomaly_str[j * 10], "%09d,", anomaly_to_thingsboard[j]);
          else sprintf(&anomaly_str[j * 10], "%09d", anomaly_to_thingsboard[j]);
        }
        anomaly_str[SAMPLES*3*10] = '\0';
        tb.sendTelemetryData("anomaly_sample",anomaly_str);
        tb.sendTelemetryData("class",clf.n_of_clusters-1);
        Serial.print(".");
        tb.loop();
      }
      Serial.println();
    }
  }

  tb.loop();
}

void INT1Event_cb() {
  mems_event = 1;
}

void getMLCStatus(uint8_t status) {
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
