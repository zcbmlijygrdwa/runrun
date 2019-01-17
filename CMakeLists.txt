cmake_minimum_required(VERSION 2.8)

project(EssentialTest)

add_compile_options(-std=c++11)

#add_library(hello_shared SHARED libHelloSLAM.cpp)
INCLUDE_DIRECTORIES(
"/usr/local/include"
)

link_directories(
"/usr/local/lib"
"/usr/X11R6/lib"
)

link_libraries(
opencv_core
opencv_calib3d
opencv_imgproc
opencv_highgui
X11
)

add_executable(main main.cpp)
