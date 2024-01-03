cmake_minimum_required(VERSION 3.2)
project(invisible_ai_takehome)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)

add_executable(takehome ${SOURCE_FILES})

