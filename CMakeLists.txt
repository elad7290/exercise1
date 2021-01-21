cmake_minimum_required(VERSION 3.17)
project(my_project)

set(CMAKE_CXX_STANDARD 14)


set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


include_directories(.)

add_executable(my_project
        anomaly_detection_util.cpp
        MainTrain.cpp
        SimpleAnomalyDetector.cpp
        timeseries.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp Server.cpp)
