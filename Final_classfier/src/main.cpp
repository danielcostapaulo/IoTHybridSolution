#include "LSM6DSOX.h"
#include <SPI.h>
#include "fft_handler.h"
#include "peakfinder1_peak.h"
#include "Models\RF_model.h"
#include "OTA.h"
#include <WiFi.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

//Thingsboard atributes
constexpr char WIFI_SSID[] = "DaniS24";
constexpr char WIFI_PASSWORD[] = "nineplusten";
constexpr char TOKEN[] = "W53xWNA8w2B7L4dONFja";
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;
constexpr size_t MAX_ATTRIBUTES = 2U;
constexpr int16_t telemetrySendInterval = 2000U;

//Thingsboard variables
WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoardSized<Default_Fields_Amount, Default_Subscriptions_Amount, MAX_ATTRIBUTES> tb(mqttClient, MAX_MESSAGE_SIZE);
constexpr char LED_MODE_ATTR[] = "ledMode";
volatile bool attributesChanged = false;
volatile int ledMode = 0;
uint32_t previousStateChange;
uint32_t previousDataSend;
constexpr std::array<const char *, 1U> CLIENT_ATTRIBUTES_LIST = {
  LED_MODE_ATTR
};

//Thingsboard functions
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
void processSetLedMode(const JsonVariantConst &data, JsonDocument &response) {
  Serial.println("Received the update RPC method");

  // Process data
  int new_mode = data;

  Serial.print("Mode to change: ");
  Serial.println(new_mode);
  StaticJsonDocument<1> response_doc;

  if (new_mode != 0 && new_mode != 1) {
    response_doc["error"] = "Unknown mode!";
    response.set(response_doc);
    return;
  }

  ledMode = new_mode;

  attributesChanged = true;

  // Returning current mode
  response_doc["newMode"] = (int)ledMode;
  response.set(response_doc);
}
const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ "setLedMode", processSetLedMode }
};

void processClientAttributes(const JsonObjectConst &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (strcmp(it->key().c_str(), LED_MODE_ATTR) == 0) {
      const uint16_t new_mode = it->value().as<uint16_t>();
      ledMode = new_mode;
    }
  }
}
const Attribute_Request_Callback<MAX_ATTRIBUTES> attribute_client_request_callback(&processClientAttributes, CLIENT_ATTRIBUTES_LIST.cbegin(), CLIENT_ATTRIBUTES_LIST.cend());

#define INT_1 1
#define GT_pin 2

//Important note: Some variables are not declared in this file but at the end of the ML model, if you cant find a variable,please check said file.

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
//FFT PEAK
#define PEAK_THRESHOLD 0
#define PEAK_INTERVAL 60
float peak_value[PEAK_N];
float peak_amps[PEAK_N];
float signal_abcissa[SAMPLES];
float peak_value_buffer[PEAK_N*3];
float peak_amps_buffer[PEAK_N*3];
PeakFinder1 FFT_peak=PeakFinder1(SAMPLE_FREQ, SAMPLES,PEAK_N, peak_value, peak_amps, FFT_amp, signal_abcissa);

//Some variables and functions to sort the values based on frequency and not amplitude
struct sort_array
{
    float freq;
    float amp;
};

int compare(const void *a,const void *b){ 
    struct sort_array *a1=(struct sort_array *) a;
    struct sort_array *a2=(struct sort_array *) b;
    if ((*a1).freq >(*a2).freq) return 1;
    else if ((*a1).freq <(*a2).freq) return -1;
    else return 0;
}
sort_array sort_peak[PEAK_N];

//This function is utilized to perform the feature extraction of the axis and insert it into the sample

void add_to_sample_to_pred(float Sample_to_pred[],int axis[],int section){ 
  //do FFT
  for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=axis[i];
  FFT.FFT();
  if(USE_PEAK){ //use peaks instead of the whole FFT
    FFT_peak.get_peaks_made_algorithm1(PEAK_THRESHOLD,PEAK_INTERVAL);
    if(SORT){ //sort based on freuqncy and not amplitude
      for(int i=0;i<PEAK_N;i++){
        sort_peak[i].freq=peak_value[i];
        sort_peak[i].amp=peak_amps[i];
      }
      qsort(sort_peak,PEAK_N,sizeof(sort_peak[0]),compare);
      for(int i=0;i<PEAK_N;i++){
        Sample_to_pred[(i+PEAK_N*(section-1))*2]=sort_peak[i].freq;
        Sample_to_pred[(i+PEAK_N*(section-1))*2+1]=sort_peak[i].amp;
      }
    }
    else{
      for(int i=0;i<PEAK_N;i++){
        Sample_to_pred[(i+PEAK_N*(section-1))*2]=peak_value[i];
        Sample_to_pred[(i+PEAK_N*(section-1))*2+1]=peak_amps[i];
      }
    }
  }
  else{
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[(i+SAMPLES*(section-1))]=float(FFT_amp[i]);
  }    
}

//Classifier

float Sample_to_pred[SAMPLES*3];
Eloquent::ML::Port::RandomForest clf;
int classifier_pred=-1;
int classifier_counter;
int current_pred;



void setup() {
  // Initalize serial connection
  Serial.begin(115200);
  delay(1000);
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount failed, please wait");
    return;
  }
  SPIFFS.format();
  pinMode(GT_pin, INPUT);
  pinMode(INT_1, OUTPUT);
  digitalWrite(INT_1, LOW);

  
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
  //FFT peak 
  for (int i=0;i<SAMPLES;i++){
    signal_abcissa[i]=float(i)*float(SAMPLE_FREQ)/(float(SAMPLES));
  }
  //Thingsboard+OTA
  InitWiFi();
  tb.disconnect();
}

void loop() {
  //--------------Thingsboard+OTA------------------
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
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }

    Serial.println("Subscribe done");

    // Request current states of client attributes
    if (!tb.Client_Attributes_Request(attribute_client_request_callback)) {
      Serial.println("Failed to request for client attributes");
      return;
    }
  }
  //check if its requesting update
  if(ledMode==1){
    tb.disconnect();
    if(checkFileFromServer()){
      performOTAUpdateFromSPIFFS();
    }
  }

  // Sending telemetry every telemetrySendInterval time
  if (millis() - previousDataSend > telemetrySendInterval) {
    previousDataSend = millis();
    tb.sendTelemetryData("temperature", random(10, 20));
    tb.sendAttributeData("rssi", WiFi.RSSI());
    tb.sendAttributeData("channel", WiFi.channel());
    tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
    tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
    tb.sendAttributeData("ssid", WiFi.SSID().c_str());
    tb.sendAttributeData("program","RF_model");
  }

  tb.loop();
//-----------------------------------------------------

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
    x_axis[sample_n]=x*10;
    y_axis[sample_n]=y*10;
    z_axis[sample_n]=z*10;
    sample_n++;
  }

  //buffers are full and ready for FFT & clasisfier
  if(sample_n==SAMPLES-1){
    //Prepare sample for classification
    FFT.Windowing(FFT_WIN_TYPE_HAMMING);
    add_to_sample_to_pred(Sample_to_pred,x_axis,1);
    add_to_sample_to_pred(Sample_to_pred,y_axis,2);
    add_to_sample_to_pred(Sample_to_pred,z_axis,3);

    //perform classifier model

    classifier_pred=clf.predict(Sample_to_pred);

    //Meta-classifier
    if (classifier_pred!=current_pred){
      classifier_counter++;
    }
    else if(classifier_counter>0){
      classifier_counter--;
    }
    if(classifier_counter==5){
      current_pred=classifier_pred;
      current_pred=0;
    }
  
    Serial.print("ACC state:");Serial.print(state);Serial.print(";Classifier state:");Serial.println(classifier_pred);
    sample_n=0;
  }
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
