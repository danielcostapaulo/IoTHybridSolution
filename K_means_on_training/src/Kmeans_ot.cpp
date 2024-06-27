#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm> 
#include "fft_handler.h"


using namespace std;

#define SAMPLES 256
#define SAMPLING_RATE 6666
#define WINDOW FFT_WIN_TYPE_HAMMING

#define n_of_samples_per_state 5000
#define test_ratio 0.7

int FFT_buffer[SAMPLES];
float FFT_amp[SAMPLES];
float signal_abcissa[SAMPLES];
FFT_handler FFT(SAMPLES,1, FFT_buffer, FFT_amp);

class Kmeans_OT{
    public:
        int n_of_samples_to_train;
        vector<vector<float>> clusters;
        vector<float> max_dist; 
        Kmeans_OT(){
            n_of_samples_to_train=50;
            swicth_to_make_new_cluster=0;
        }
        Kmeans_OT(int n_of_samples){
            n_of_samples_to_train=n_of_samples;
            swicth_to_make_new_cluster=0;
        }
        int predict(vector<float> sample_to_pred){
            float min_dist=0;
            float min_aux=0;
            int result=-1;
            float dist=0;
            if(clusters.empty()){
                swicth_to_make_new_cluster++;
                result=-1;
                anomaly_samples.push_back(sample_to_pred);
            }
            else{
                for(int cluster_n=0;cluster_n<clusters.size();cluster_n++){
                    dist=euclidean_distance(clusters[cluster_n],sample_to_pred);
                    if(min_aux==0){
                        min_dist=dist;
                        min_aux=1;
                        result=cluster_n;
                    }
                    if(min_dist>dist){
                        min_dist=dist;
                        result=cluster_n;
                    }
                }
                if(min_dist>max_dist[result]){
                    result=-1;
                    swicth_to_make_new_cluster++;
                    anomaly_samples.push_back(sample_to_pred);
                }
                else if(swicth_to_make_new_cluster>0){
                    swicth_to_make_new_cluster--;
                    anomaly_samples.erase(anomaly_samples.begin());
                } 
            }
            if(swicth_to_make_new_cluster>=n_of_samples_to_train){
                make_new_cluster();
                result=-2;
            }
            return result;
        }
    private:
        vector<vector<float>> anomaly_samples;
        int swicth_to_make_new_cluster;
        float euclidean_distance(vector<float> a,vector<float> b){
            float distance=0;
            for(int feature_n=0;feature_n<int(a.size());feature_n++){
                distance+=(a[feature_n]-b[feature_n])*(a[feature_n]-b[feature_n]);
            }
            distance=sqrt(distance);
            return distance;
        }
        float cosine_distance(vector<float> a,vector<float> b){
            float distance=0;
            float upper_eq=0;
            float bottom_a=0;
            float bottom_b=0;
            for(int feature_n=0;feature_n<int(a.size());feature_n++){
                upper_eq+=a[feature_n]*b[feature_n];
                bottom_a+=a[feature_n]*a[feature_n];
                bottom_b+=b[feature_n]*b[feature_n];
            }
            bottom_a=sqrt(bottom_a);
            bottom_b=sqrt(bottom_b);
            return upper_eq/(bottom_a*bottom_b);
        }
        float jaccard_distance(vector<float> a,vector<float> b){
            float jaccard_index=0;
            for(int i=1;i<a.size();i++){
                if(a[i]>=a[i-1]){
                    a[i-1]=a[i];
                }
                if(b[i]>=b[i-1]){
                    b[i-1]=b[i];
                }
            }
            for(int i=0;i<a.size();i++){
                if(a[i]==b[i]) jaccard_index++;
            }
            jaccard_index=jaccard_index/(a.size()+b.size()-jaccard_index);
            return 1-jaccard_index;
        }
        float tanimoto_distance(vector<float> a,vector<float> b){
            float dot=0;
            float a_sqrt=0;
            float b_sqrt=0;
            for(int i=0;i<a.size();i++){
                dot+=a[i]*b[i];
                a_sqrt+=a[i]*a[i];
                b_sqrt+=b[i]*b[i];
            }
            return dot/(a_sqrt+b_sqrt-dot);
        }
        float bad_peak_distance(vector<float> a,vector<float> b){
            
        }
        void make_new_cluster(){
            vector<float> new_cluster(anomaly_samples[0].size(),0);
            vector<float> distances;
            for(int sample_n=0;sample_n<int(n_of_samples_to_train);sample_n++){
                for(int feature_n=0;feature_n<int(anomaly_samples[sample_n].size());feature_n++){
                    new_cluster[feature_n]+=anomaly_samples[sample_n][feature_n];
                }
            }
            for(int feature_n=0;feature_n<int(anomaly_samples[0].size());feature_n++){
                new_cluster[feature_n]=float(new_cluster[feature_n]/n_of_samples_to_train);
            }
            //get the max_dist of this cluster
            float new_max_dist=-1;
            float dist=0;
            for(int sample_n=0;sample_n<int(n_of_samples_to_train);sample_n++){
                dist=euclidean_distance(anomaly_samples[sample_n],new_cluster);
                distances.push_back(dist);
            }
            sort(distances.begin(),distances.end());
            new_max_dist=distances[47];
            //put cluster on vector of clusters and the max distance 
            //cout<<new_max_dist<<endl;
            //for(int i=0;i<distances.size();i++) cout<<distances[i]<<endl;
            clusters.push_back(new_cluster);
            max_dist.push_back(new_max_dist);
            swicth_to_make_new_cluster=0;
            anomaly_samples.clear();
        }
};


int main(){
    //do the frequency values, can be used for peak detection
    for (int i=0;i<SAMPLES;i++){
      signal_abcissa[i]=float(i)*float(SAMPLING_RATE)/(float(SAMPLES));
    }
    ifstream raw_file;
    chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/actual_new_motor");
    //first get all of the features using FFT
    vector<vector<vector<float>>> features;
    DIR *dr=opendir(".");
    struct dirent *en;
    cout<<"Loading data";
    if (dr) {
      while ((en = readdir(dr)) != NULL) {
        if(en->d_name[0]=='s' || en->d_name[0]=='l'){      
            chdir("C://Users/danip/OneDrive/Desktop/IST-vibration/Data/actual_new_motor");
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
            vector<vector<float>> all_FFT_of_file;
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
                        cout<<"not a number"<<endl;
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
            vector<float> all_axis_FFT(SAMPLES*3);
            int axis=0;
            int samples_counter=0;
            j=0;
            int n_of_samples_done=0;
            int n_of_features_done=0;
            while(n_of_features_done<n_of_samples_per_state && n_of_samples_done<int(raw_data.size())){
                FFT_buffer[j]=(raw_data[n_of_samples_done][axis])*1000;
                n_of_samples_done++;
                j++;
                if(j==SAMPLES-1){
                    FFT.Windowing(WINDOW);
                    FFT.FFT();
                    int FFT_aux[SAMPLES];
                    for(int k=0;k<SAMPLES;k++){
                        
                    }
                    for(int k=0;k<SAMPLES;k++){
                        all_axis_FFT[(axis*SAMPLES)+k]=FFT_amp[k];
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


    vector<vector<vector<float>>> train_DS;
    vector<vector<vector<float>>> test_DS;
    vector<vector<float>> new_FFT;
    for(int i=0;i<int(features.size());i++){
        for(int j=0;j<int(features[i].size());j++){
            if(j==int(features[i].size()*test_ratio)){
                test_DS.push_back(new_FFT);
                new_FFT.clear();
            }
            new_FFT.push_back(features[i][j]);
        }
        train_DS.push_back(new_FFT);
        new_FFT.clear();
    }
    cout<<"Train dataset size:"<<train_DS[0].size()*train_DS.size()<<endl;
    cout<<"Test dataset size:"<<test_DS[0].size()*test_DS.size()<<endl;

    cout<<"Now to perform the kmeans training"<<endl;

    //-----------------------------------------------------"kmeans" training--------------------------------------------------
    int err=0;
    int corr=0;
    int result;
    Kmeans_OT clf(50);
    for(int state=0;state<train_DS.size();state++){
        for(int sample_n=0;sample_n<train_DS[state].size();sample_n++){
            result=clf.predict(train_DS[state][sample_n]);
            if(result==-2) cout<<"Doing cluster for state:"<<state<<endl;
        }
    }
    vector<int>predicted;
    vector<int>real;
    for(int state=0;state<test_DS.size();state++){
        for(int sample_n=0;sample_n<test_DS[state].size();sample_n++){
            result=clf.predict(test_DS[state][sample_n]);
            predicted.push_back(result);
            real.push_back(state);
            if(result!=state) err++;
            else corr++;
        }
    }
    int cm1[train_DS.size()+1][train_DS.size()]={};
    for(int i=0;i<predicted.size();i++){
        if(predicted[i]==-1) cm1[train_DS.size()][real[i]]++;
        else if(predicted[i]!=-2) cm1[predicted[i]][real[i]]++;
    }
    for(int i=0;i<train_DS.size()+1;i++){
        for(int j=0;j<train_DS.size();j++){
            cout<<cm1[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"Accuracy:"<<(float(corr)/(float(err)+float(corr)))*100<<"%"<<endl;
}