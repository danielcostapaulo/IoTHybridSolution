#include "Kmeans.h"
#include <cmath>
#include <algorithm>
#include <iostream>


Kmeans_OT::Kmeans_OT(){
    n_of_samples_to_train=50;
    switch_to_make_new_cluster=0;
    for(int i=0;i<50;i++){
        anomaly_samples[i][0]=-1;
    }
    for(int i=0;i<N_OF_CLUSTERS;i++){
        clusters[i][0]=-1;
    }
}
Kmeans_OT::Kmeans_OT(int number_of_anomalies){
    n_of_samples_to_train=50;
    switch_to_make_new_cluster=0;
    for(int i=0;i<50;i++){
        anomaly_samples[i][0]=-1;
    }
    for(int i=0;i<N_OF_CLUSTERS;i++){
        clusters[i][0]=-1;
    }
}
int* Kmeans_OT::get_anomaly(int anomaly_n){
    return anomaly_samples[anomaly_n];
}
int Kmeans_OT::predict(int sample_to_pred[SAMPLES*3]){
    float min_dist=0;
    bool min_aux=0;
    int result=-1;
    float dist=0;
    //There are no clusters, so all of samples are anomalies for the first cluster
    if(n_of_used_clusters==0){
        switch_to_make_new_cluster++;
        result=-1; 
        for(int features=0;features<SAMPLES*3;features++){
            anomaly_samples[next_anomaly][features]=sample_to_pred[features];
        }
        next_anomaly++;
    }
    else{
        for(int cluster_n=0;cluster_n<n_of_used_clusters;cluster_n++){
            dist=Kmeans_OT::eucledian_distance(clusters[cluster_n],sample_to_pred);
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
            switch_to_make_new_cluster++;
            for(int features=0;features<SAMPLES*3;features++){
                anomaly_samples[next_anomaly][features]=sample_to_pred[features];
            }
            next_anomaly++;
            if(next_anomaly==n_of_samples_to_train){
                next_anomaly=0;
            }
        }
        else if(switch_to_make_new_cluster>0){
            switch_to_make_new_cluster--;
        } 
    }
    if(switch_to_make_new_cluster>=n_of_samples_to_train){
        if(n_of_used_clusters<N_OF_CLUSTERS){
            make_new_cluster();
            return -2;
        }
    }
    return result;
}

float Kmeans_OT::eucledian_distance(int a[SAMPLES*3],int b[SAMPLES*3]){
    float distance=0;
    for(int feature_n=0;feature_n<SAMPLES*3;feature_n++){
        distance=distance+float(float(a[feature_n]-b[feature_n])*float(a[feature_n]-b[feature_n]));
    }
    distance=sqrt(distance);
    return distance;
}

void Kmeans_OT::make_new_cluster(){
    float new_cluster[SAMPLES*3]={0};
    float distances[50];
    for(int sample_n=0;sample_n<n_of_samples_to_train;sample_n++){
        for(int feature_n=0;feature_n<SAMPLES*3;feature_n++){
            new_cluster[feature_n]+=float(anomaly_samples[sample_n][feature_n]);
        }
    }
    for(int feature_n=0;feature_n<SAMPLES*3;feature_n++){
        new_cluster[feature_n]=new_cluster[feature_n]/float(n_of_samples_to_train);
    }
    for(int features=0;features<SAMPLES*3;features++){
        clusters[n_of_used_clusters][features]=int(new_cluster[features]);
    }
    //get the max_dist of this cluster
    float new_max_dist=-1;
    float dist=0;
    for(int sample_n=0;sample_n<int(n_of_samples_to_train);sample_n++){
        dist=eucledian_distance(anomaly_samples[sample_n],clusters[n_of_used_clusters]);
        distances[sample_n]=dist;
    }
    std::sort(std::begin(distances),std::end(distances));
    float mean=0;
    for(int sample_n=0;sample_n<50;sample_n++){
        mean=mean+distances[sample_n];
    }
    mean=mean/50;
    float std=0;
    for(int i=0;i<50;i++){
        std=std+(distances[i]-mean)*(distances[i]-mean);
    }
    std=std/50;
    std=sqrt(std);
    int last_element=0;
    for(int i=0;i<50;i++){
        if(distances[i]<mean+1.5*std){
            last_element=i;
        }
    }
    new_max_dist=distances[last_element];

    max_dist[n_of_used_clusters]=new_max_dist;
    n_of_used_clusters++;
    switch_to_make_new_cluster=0;
    next_anomaly=0;
}

