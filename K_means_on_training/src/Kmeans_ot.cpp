#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm> 
#include "fft_handler.h"
#include "Kmeans.h"

using namespace std;

#define SAMPLES 256
#define SAMPLING_RATE 6666
#define WINDOW FFT_WIN_TYPE_HAMMING

#define n_of_samples_per_state 5000
#define test_ratio 0.3

int FFT_buffer[SAMPLES];
float FFT_amp[SAMPLES];
float signal_abcissa[SAMPLES];
FFT_handler FFT(SAMPLES,1, FFT_buffer, FFT_amp);

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
                    int FFT_aux[SAMPLES];
                    for(int k=0;k<SAMPLES;k++){
                        if(FFT_amp[k]!=0) all_axis_FFT[(axis*SAMPLES)+k]=int(FFT_amp[k]);
                        else all_axis_FFT[(axis*SAMPLES)+k]=0;
                        
                    }
                    axis++;
                    n_of_features_done++;
                    j=0;
                    n_of_samples_done=samples_counter*SAMPLES;
                    if(axis==3){
                        axis=0;
                        samples_counter++;
                        all_FFT_of_file.push_back(all_axis_FFT);
                    }
                }
            }
            cout<<".";
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
    Kmeans_OT clf(50);
    vector <int> cluster_to_state;
    int sample[SAMPLES*3];
    for(int state=0;state<train_DS.size();state++){
        for(int sample_n=0;sample_n<train_DS[state].size();sample_n++){
            std::copy(train_DS[state][sample_n].begin(), train_DS[state][sample_n].end(), sample);
            result=clf.predict(sample);
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
    for(int i=0;i<cluster_to_state.size();i++)cout<<cluster_to_state[i]<<endl;
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
            predicted.push_back(curr_res);
            real.push_back(state);
            if(curr_res!=cluster_to_state[curr_res]) err++;
            else corr++;
        }
    }
    int cm1[cluster_to_state.size()+1][train_DS.size()]={};
    for(int i=0;i<predicted.size();i++){
        if(predicted[i]==-1) cm1[cluster_to_state.size()][real[i]]++;
        else if(predicted[i]!=-2) cm1[predicted[i]][real[i]]++;
    }
    for(int i=0;i<cluster_to_state.size()+1;i++){
        for(int j=0;j<train_DS.size();j++){
            cout<<cm1[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"Accuracy:"<<(float(corr)/(float(err)+float(corr)))*100<<"%"<<endl;
    }
}