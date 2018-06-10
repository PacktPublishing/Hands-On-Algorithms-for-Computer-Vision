cmake_minimum_required(VERSION 3.1)

project(CvHueVisualizer)

set(OpenCV_DIR $ENV{OPENCV_PATH})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
