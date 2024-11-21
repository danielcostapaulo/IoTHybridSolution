Most of this project utilizes the features available in the frequency domain of the vibration signal in the ML models.

To obtain these values, the ApproxFFT(https://www.instructables.com/ApproxFFT-Fastest-FFT-Function-for-Arduino/) algorithm is used to transform the time domain signals to the frequency domain.

The frequency domain of the signal can, however, be too large and most of the information of this signal can be obtained by its peaks. The algorithms to obtain these peaks were done in https://github.com/RicardoFebra/IST-Thesis-LowPowerEdgeML4IoT/tree/main.

The main code(extract_features.cpp) has some variables that should be taken into account, namely:
* The dataset whose features will be extracted.
* The peak algorithm to use and its parameters.
* The parameters of the ApproxFFT algorithm.

To execute the code, it's advised to execute the Makefile available in this folder. This will generate a .exe file that can be executed to perform this task.
