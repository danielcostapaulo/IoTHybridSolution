#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include "fft_handler.h"

using namespace std;

#define FILE_NAME "idle_1"

#define SAMPLES 256
#define SAMPLING_RATE 26
float FFT_buffer[SAMPLES];
float FFT_amp[SAMPLES];
FFT_handler FFT=FFT_handler(SAMPLES,1, FFT_buffer, FFT_amp);

 
int main(){
    ofstream feature_file;          
    ifstream raw_file;
    chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/new_motor");
    raw_file.open(string(FILE_NAME)+".txt");
     if (!raw_file.is_open()) { 
        cerr << "Error opening the file!" << endl; 
        return 1; 
    } 
    chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/new_motor/Features");
    feature_file.open(string(FILE_NAME)+"_Feature.txt",ios::out);
    string line;
    string number_str;
    vector<vector<float>> raw_data;
    int i,j;
    float numb;
    while(getline(raw_file,line)){
        vector <float> aux;
        i=0;
        j=0;
        while(line[i]!='\0' && j<=2){
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
        if(axis==0) feature_file<<"x: "<<endl;
        else if(axis==1) feature_file<<"y:  "<<endl;
        else feature_file<<"z:  "<<endl;
        j=0;
        for(int i=0;i<raw_data.size();i++){
            FFT_buffer[j]=raw_data[i][axis]*1000;
            j++;
            if(j==SAMPLES-1){
                FFT.Windowing(FFT_WIN_TYPE_HAMMING);
                FFT.FFT();
                for(int k=0;k<j;k++){
                    feature_file<<FFT_amp[k]<<" ";
                }
                feature_file<<endl;
                j=0;
            }
        }
    }
    feature_file.close();
    raw_file.close();

}