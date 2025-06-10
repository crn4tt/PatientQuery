cmake_minimum_required(VERSION 3.10)

project(Patient_Queue)

set(CMAKE_CXX_STANDARD 17)



set(SOURCES
    src/main.cpp
)

add_executable(Patient_Queue ${SOURCES})
