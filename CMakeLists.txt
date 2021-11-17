cmake_minimum_required(VERSION 3.20)
project(CompGraph_lab7)

set(CMAKE_CXX_STANDARD 20)

add_executable(CompGraph_lab7 main.cpp ClipSegments.cpp ClipSegments.h)

target_link_libraries(CompGraph_lab7 -lOpenGL32 -lfreeGLUT -lGLU32)
