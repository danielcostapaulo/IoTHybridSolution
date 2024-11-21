# IoTHybridSolution

"IoTHybridSolution" was the project of my Thesis, which I completed in Lisbon, Portugal, at Instituto Superior Tecnico for my Master's in Electrical and Computer Engineering.

This project uses vibration data to monitor a machine in a non-intrusive, power-efficient, and flexible solution. To perform this task, the ESP32-S3 chip was used, alongside the LSM6DSOX IMU. This project inserts itself in Edge Computing, in which the processing of big data (vibration in this project) is done in a low-resource MCU. The main contributions of this project were:
* Implementing an activity detector that works purely in the IMU, so that the main board can sleep when it is not needed, saving battery life.
* Developing a hybrid solution that can either learn about the machine and obtain the necessary data to train and test supervised ML without human interaction or have the ML model deployed and performing continous monitoring. This solution was done either "offline" utilizing Serial Port to communicate with the device or "online", utilizing Thingsboard to store data and communicate with the device and Github to store the necessary firmware.

This project was performed in 3 phases:
1. Develop a device that can determine if a machine is active or not. The LSM6DSOX IMU has a Machine Learning Core that can implement a Decision Tree capable of doing this task while the MCU works as a communication device.
2. Train and test machine learning models that will identify the many states a machine can have when it's performing work. These models are then deployed into the edge device for continuous monitoring.
3. Implement a learning mode into the device in which its function is to develop a supervised dataset that can then be trained and tested for continuous monitoring.

The most important folders are:
* OnOff:  This folder contains the code that implements the activity detector. This code uses models that are generated in the UNICO-GUI program(https://www.st.com/en/development-tools/unico-gui.html).
* Final_classifier(offline and Thingsboard): Contains code that implements continuous monitoring.
* CC(offline and Thingsboard): Contains code that implements the learning mode.
* HighLevel: All of the code is done in Python, from plotting and analysis of data and results to training and testing of ML models, among others.

The Auxiliary folder contains subfolders that were chunks of code that are not relevant for the final solution.
The Firmware folder contains the firmware that can be used to update the MCU trough OTA updates.
