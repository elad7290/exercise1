cmake_minimum_required(VERSION 3.17)
project(my_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(my_project
        anomaly_detection_util.cpp
        MainTrain.cpp
        SimpleAnomalyDetector.cpp
        timeseries.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp)
