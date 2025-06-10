cmake_minimum_required(VERSION 3.10)

project(Patient_Queue)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    patient_queue.cpp
)

add_executable(Patient_Queue ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(Patient_Queue PRIVATE Threads::Threads)
