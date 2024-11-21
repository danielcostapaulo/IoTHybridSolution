    #include <iostream>
    #include <cmath>
    using namespace std;

    class KMeans{
        public:
            int max_dist[9]={7,8,6,7,7,8,10,6,7};
            int clusters[9][3]={{3,0,0},{-5,-3,-1},{-2,7,-1},{-2,0,1},{2,-3,-1},{-3,-2,0},{-1,1,2},{2,0,0},{-3,4,0}};
            int predict(float* sample);
    };
    int KMeans::predict(float * sample){
        float min_distance=0;
        bool min_aux=1;
        int res=0;
        for(int i=0;i<int(9);i++){
            float distance=0;
            for(int j=0;j<int(3);j++){
                distance=distance+sqrt((sample[j]-KMeans::clusters[i][j])*(sample[j]-KMeans::clusters[i][j]));
            }
            cout<<distance<<endl;
            if(min_aux==1){
                min_distance=distance;
                min_aux=0;
                res=i;
            }
            else{
                if(min_distance>distance){
                    min_distance=distance;
                    res=i;
                }
            }
        }
        if(min_distance>max_dist[res]){
            return -1;
        }
        return res;
    }
    
    #define USE_PEAK 1 
#define PEAK_N 6
#define USE_STD 0 
#define SORT 1 