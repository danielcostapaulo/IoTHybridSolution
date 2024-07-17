#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include "fft_handler.h"
#include "peakfinder1_peak.h"
#include "peak_handler.h"
#include "zscore_peak.h"

using namespace std;

#define SAMPLES 256
#define SAMPLING_RATE 6666
#define PEAK_N 6
#define PEAK_THRESHOLD 0
#define PEAK_INTERVAL 60
#define WINDOW FFT_WIN_TYPE_HAMMING
int FFT_buffer[SAMPLES];
float FFT_amp[SAMPLES];
float peak_value[PEAK_N];
float peak_amps[PEAK_N];
float signal_abcissa[SAMPLES];
FFT_handler FFT(SAMPLES,1, FFT_buffer, FFT_amp);
PeakFinder1 FFT_peak=PeakFinder1(SAMPLING_RATE, SAMPLES,PEAK_N, peak_value, peak_amps, FFT_amp, signal_abcissa);
//PeakHandler FFT_peak=PeakHandler(SAMPLING_RATE, PEAK_N,SAMPLES, peak_value, peak_amps, FFT_amp, signal_abcissa);


 
int main(){
    for (int i=0;i<SAMPLES;i++){
      signal_abcissa[i]=float(i)*float(SAMPLING_RATE)/(float(SAMPLES));
    }
    ofstream feature_file;          
    ifstream raw_file;
    chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/corrected_new_motor");
    DIR *dr=opendir(".");
    struct dirent *en;
    if (dr) {
      while ((en = readdir(dr)) != NULL) {
        if(en->d_name[0]=='s' || en->d_name[0]=='l'){      
            chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/corrected_new_motor");
            //doing file name operations-----------------
            char raw_file_name[100];
            strcpy(raw_file_name,en->d_name);
            raw_file.open(raw_file_name);
            if (!raw_file.is_open()) { 
                cerr << "Error opening the file!" << endl; 
                return 1; 
            } 
            char *dot=strrchr(raw_file_name,'.');
            *dot='\0';
            char feature_file_name[100];
            strcpy(feature_file_name,raw_file_name);
            strcat(feature_file_name,"_Feature.txt");
            chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/corrected_new_motor/Features");
            cout<<"Performing feature extraction on file:";
            cout<<feature_file_name<<endl;
            feature_file.open(feature_file_name,ios::out);
            //---------------------------------------------
            string line;
            string number_str;
            vector<vector<float>> raw_data;
            int i,j;
            float numb;

            while(getline(raw_file,line)){
                vector <float> aux;
                i=0;
                j=0;
                while((line[i]!='\n' || line[i]!='\0') && j<=2){
                    while(line[i]!=' '){
                        number_str+=line[i];
                        i++;
                    }
                    string::size_type sz;
                    try{
                        numb=stod(number_str,&sz);
                        aux.push_back(numb);
                    }
                    catch(const std::exception& e){
                        
                    }
                    number_str.clear();
                    i++;
                    j++;
                }
                if(aux.size()!=0){
                    raw_data.push_back(aux);
                }
            }
            for(int axis=0;axis<3;axis++){
                float grav=0;
                if(axis==0){
                    feature_file<<"x: "<<endl;
                } 
                else if(axis==1){
                    feature_file<<"y:  "<<endl;
                } 
                else {
                    feature_file<<"z:  "<<endl;
                }
                j=0;
                for(int i=0;i<int(raw_data.size());i++){
                    grav+=raw_data[i][axis];
                }
                grav=grav/raw_data.size();
                for(int i=0;i<int(raw_data.size());i++){
                    FFT_buffer[j]=(raw_data[i][axis]-grav)*1000;
                    j++;
                    if(j==SAMPLES-1){
                        float mean=0;
                        float std=0;
                        for(int k=0;k<j;k++){
                            mean+=(raw_data[i-k][axis]-grav)*(raw_data[i-k][axis]-grav);
                        }
                        mean=mean/j;
                        mean=sqrt(mean);
                        for(int k=0;k<j;k++){
                            std+=((raw_data[i-k][axis]-grav)-mean)*((raw_data[i-k][axis]-grav)-mean);
                        }
                        std=std/(j-1);
                        std=sqrt(std);
                        FFT.Windowing(WINDOW);
                        FFT.FFT();
                        FFT_peak.get_peaks_made_algorithm1(PEAK_THRESHOLD,PEAK_INTERVAL);
                        //FFT_peak.PEAKS();
                        for(int k=0;k<j;k++){
                            feature_file<<FFT_amp[k]<<" ";
                        }
                        feature_file<<endl;
                        for(int k=0;k<PEAK_N;k++){
                            feature_file<<peak_value[k]<<" ";
                        }
                        feature_file<<endl;
                        for(int k=0;k<PEAK_N;k++){
                            feature_file<<peak_amps[k]<<" ";
                        }
                        feature_file<<endl;
                        feature_file<<mean<<" "<<std<<endl;
                        j=0;
                    }
                }
            }
            feature_file.close();
            raw_file.close();
        }
      }
    }
    closedir(dr);
}