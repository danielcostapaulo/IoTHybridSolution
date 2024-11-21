#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm> 
#include "fft_handler.h"
#include "Kmeans.h"
#include "peakfinder1_peak.h"


using namespace std;

#define SAMPLES 256
#define SAMPLING_RATE 6666
#define WINDOW FFT_WIN_TYPE_HAMMING

#define n_of_samples_per_state 100000000
#define test_ratio 0.3

int FFT_buffer[SAMPLES];
float FFT_amp[SAMPLES];
float signal_abcissa[SAMPLES];
FFT_handler FFT(SAMPLES,1, FFT_buffer, FFT_amp);

//FFT PEAK
#define PEAK_N 12
#define PEAK_THRESHOLD 0
#define PEAK_INTERVAL 60
float peak_value[PEAK_N];
float peak_amps[PEAK_N];
float peak_value_buffer[PEAK_N*3];
float peak_amps_buffer[PEAK_N*3];
PeakFinder1 FFT_peak=PeakFinder1(SAMPLING_RATE, SAMPLES,PEAK_N, peak_value, peak_amps, FFT_amp, signal_abcissa);



int main(){
    //do the frequency values, can be used for peak detection
    for (int i=0;i<SAMPLES;i++){
      signal_abcissa[i]=float(i)*float(SAMPLING_RATE)/(float(SAMPLES));
    }
    ifstream raw_file;
    chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/corrected_new_motor");
    //first get all of the features using FFT
    vector<vector<vector<int>>> features;
    DIR *dr=opendir(".");
    struct dirent *en;
    cout<<"Loading data";
    if (dr) {
      while ((en = readdir(dr)) != NULL) {
        if(en->d_name[0]=='s' || en->d_name[0]=='l'){      
            //open file
            char raw_file_name[100];
            strcpy(raw_file_name,en->d_name);
            raw_file.open(raw_file_name);
            if (!raw_file.is_open()) { 
                cout<<"Error opening the file!"<< endl; 
                return 1; 
            }
            string line;
            string number_str;
            vector<vector<float>> raw_data;
            vector<vector<int>> all_FFT_of_file;
            vector<vector<int>> all_peak_of_file;
            int i,j;
            float numb;
            while(getline(raw_file,line)){
                vector <float> aux;
                i=0;
                j=0;
                while((line[i]!='\n' || line[i]!='\0') && j<=2){
                    //get values from str to a matrix with rows of samples and columns of axis
                    while(isdigit(line[i]) || line[i]=='-' || line[i]=='.'){
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
            raw_file.close();
            //get the gravity value. Its used DC but in an actual implementation, some better method should be used
            float grav[3]={0};
            for (int axis=0;axis<3;axis++){
                for(int i=0;i<int(raw_data.size());i++){
                    grav[axis]+=raw_data[i][axis];
                }
                grav[axis]=grav[axis]/raw_data.size();
            }
            vector<int> all_axis_FFT(SAMPLES*3);
            vector<int> all_axis_PEAK(PEAK_N*2*3);
            int axis=0;
            int samples_counter=0;
            j=0;
            int n_of_samples_done=0;
            int n_of_features_done=0;
            while(n_of_features_done<n_of_samples_per_state && n_of_samples_done<int(raw_data.size())){
                FFT_buffer[j]=(raw_data[n_of_samples_done][axis]);
                FFT_buffer[j]=FFT_buffer[j]*FFT_buffer[j];
                n_of_samples_done++;
                j++;
                if(j==SAMPLES-1){
                    FFT.Windowing(WINDOW);
                    FFT.FFT();
                    FFT_peak.get_peaks_made_algorithm1(PEAK_THRESHOLD,PEAK_INTERVAL);
                    int FFT_aux[SAMPLES];
                    for(int k=0;k<SAMPLES;k++){
                        if(FFT_amp[k]!=0) all_axis_FFT[(axis*SAMPLES)+k]=int(FFT_amp[k]);
                        else all_axis_FFT[(axis*SAMPLES)+k]=0;
                    }
                    for(int k=0;k<PEAK_N;k++){
                        all_axis_PEAK[(axis*PEAK_N+(k*2))]=int(peak_value[k]);
                        all_axis_PEAK[(axis*PEAK_N+(k*2)+1)]=int(peak_amps[k]);
                    }
                    axis++;
                    n_of_features_done++;
                    j=0;
                    n_of_samples_done=samples_counter*SAMPLES;
                    if(axis==3){
                        axis=0;
                        samples_counter++;
                        all_FFT_of_file.push_back(all_axis_PEAK);
                    }
                }
            }
            cout<<".";
            cout<<all_FFT_of_file.size()<<endl;
            features.push_back(all_FFT_of_file);
        }
      }
    }
    cout<<endl;


    //now all of the Features are in a matrix of state->samples->features
    //we need to seperate between training and testing sets


    vector<vector<vector<int>>> train_DS;
    vector<vector<vector<int>>> test_DS;
    vector<vector<int>> new_FFT;
    for(int i=0;i<int(features.size());i++){
        for(int j=0;j<int(features[i].size());j++){
            if(j==int(features[i].size()*test_ratio) && test_ratio!=0){
                test_DS.push_back(new_FFT);
                new_FFT.clear();
            }
            new_FFT.push_back(features[i][j]);
        }
        train_DS.push_back(new_FFT);
        new_FFT.clear();
    }
    cout<<"Train dataset size:"<<train_DS[0].size()*train_DS.size()<<endl;
    if(test_ratio!=0) cout<<"Test dataset size:"<<test_DS[0].size()*test_DS.size()<<endl;

    cout<<"Now to perform the kmeans training"<<endl;

    //-----------------------------------------------------"kmeans" training--------------------------------------------------
    int err=0;
    int corr=0;
    int result;
    Kmeans_OT clf;
    vector <int> cluster_to_state;
    int sample[SAMPLES*3];
    for(int state=0;state<train_DS.size();state++){
        for(int sample_n=0;sample_n<train_DS[state].size();sample_n++){
            std::copy(train_DS[state][sample_n].begin(), train_DS[state][sample_n].end(), sample);
            result=clf.predict(sample);
            cout<<result;
            if(result==-2){
                cluster_to_state.push_back(state);
                cout<<"Doing cluster for state:"<<state<<endl;
                err-=100;
            } 
            else if(result!=-1){
                if(state==cluster_to_state[result]) corr++;
                else err++;
            }
            else err++;
        }
    }
    vector<int> real;
    vector<int> predicted;
    cout<<"Accuracy:"<<(float(corr)/(float(err)+float(corr)))*100<<"%"<<endl;
    if(test_ratio!=0){
    corr=0;
    err=0;

    int curr_res=-1;
    int res_counter=0;
    for(int state=0;state<test_DS.size();state++){
        for(int sample_n=0;sample_n<test_DS[state].size();sample_n++){
            std::copy(test_DS[state][sample_n].begin(), test_DS[state][sample_n].end(), sample);
            result=clf.predict(sample);
            if(curr_res!=result) res_counter++;
            else if(res_counter>0) res_counter--;
            if(res_counter==5){
                curr_res=result;
                res_counter=0;
            } 
            //cout<<"result:"<<result<<";current result:"<<curr_res<<";counter:"<<res_counter<<endl;
            if(result!=-2){
                predicted.push_back(result);
            }
            else{     
                predicted.push_back(-1);
                cout<<"did cluster for state"<<state<<endl;
                cluster_to_state.push_back(state);
            }
            real.push_back(state);
            if(result!=cluster_to_state[curr_res]) err++;
            else corr++;
        }
    }
    int dim=0;
    cout<<train_DS.size()<<endl;
    cout<<cluster_to_state.size()<<endl;
    if (train_DS.size()>cluster_to_state.size())dim=train_DS.size();
    else dim=cluster_to_state.size();
    dim++;

    for(int i=0;i<cluster_to_state.size();i++)cout<<cluster_to_state[i]<<endl;
    int cm1[dim][dim]={};
    for(int i=0;i<predicted.size();i++){
        if(predicted[i]==-1) cm1[dim-1][real[i]]++;
        else if(predicted[i]!=-2) cm1[predicted[i]][real[i]]++;
    }
    for(int j=0;j<dim;j++){
        cout<<"[";
        for(int i=0;i<dim;i++){
            cout<<cm1[i][j];
            if(i!=dim-1) cout<<",";
        }
        cout<<"],"<<endl;
    }
    cout<<"Accuracy:"<<(float(corr)/(float(err)+float(corr)))*100<<"%"<<endl;
    }
}