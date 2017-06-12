cmake_minimum_required(VERSION 3.0)
project(TargetDetectionSandbox)

find_package( OpenCV HINTS /usr/local/opt/opencv3 )

set(CMAKE_CXX_STANDARD 11)

add_executable(ConcentricDetector ConcentricDetector.cpp)

add_executable(BlobDetector BlobDetector.cpp)

target_link_libraries( ConcentricDetector ${OpenCV_LIBS} )

target_link_libraries( BlobDetector ${OpenCV_LIBS} )