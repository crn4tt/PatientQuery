cmake_minimum_required(VERSION 3.10)

project(Patient_Queue)

set(CMAKE_CXX_STANDARD 17)



set(SOURCES
src/DataBaseWorker.cpp
src/Patient.cpp
src/Visit.cpp
src/PatientQueue.cpp
src/Doctor.cpp
src/main.cpp
)

add_executable(Patient_Queue ${SOURCES})
target_include_directories(Patient_Queue PRIVATE ${CMAKE_SOURCE_DIR}/include)
