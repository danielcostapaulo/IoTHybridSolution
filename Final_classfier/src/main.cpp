#include "LSM6DSOX.h"
#include <SPI.h>
#include "fft_handler.h"
#include "Models\RF_model.h"


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
}

void loop() {
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
    //make FFT of all axis
    FFT.Windowing(FFT_WIN_TYPE_HAMMING);
    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=x_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i]=float(FFT_amp[i]);

    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=y_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i+SAMPLES]=float(FFT_amp[i]);
    
    for(int i=0;i<SAMPLES;i++) FFTbuffer[i]=z_axis[i];
    FFT.FFT();
    for(int i=0;i<SAMPLES;i++) Sample_to_pred[i+2*SAMPLES]=float(FFT_amp[i]);
    //perform classifier model

    classifier_pred=clf.predict(Sample_to_pred);
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
