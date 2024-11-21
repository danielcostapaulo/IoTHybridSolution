#ifndef KMEANS_H
#define KMEANS_H

#define SAMPLES 256

class Kmeans_OT{
  public:
    int n_of_samples_to_train;
    int clusters[30][SAMPLES*3];
    float max_dist[30];
    int n_of_clusters=0;
    Kmeans_OT();
    Kmeans_OT(int number_of_anomalies);
    int predict(int sample_to_pred[SAMPLES*3]);
  private:
    int switch_to_make_new_cluster;
    int last_anomaly=0;
    int next_anomaly=0;
    int anomaly_samples[50][SAMPLES*3];
    float eucledian_distance(int a[SAMPLES*3],int b[SAMPLES*3]);
    void make_new_cluster();
};

#endif