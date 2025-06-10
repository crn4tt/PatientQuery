cmake_minimum_required(VERSION 3.10)

project(Patient_Queue)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    new_src/Doctor.cpp
    new_src/Patient.cpp
    new_src/ConnectionHandlerClient.cpp
    new_src/ConnectionHandlerDoctor.cpp
    new_src/PatientController.cpp
    new_src/DatabaseWorker.cpp
    new_src/main.cpp
)

add_executable(Patient_Queue ${SOURCES})

target_include_directories(Patient_Queue PRIVATE ${CMAKE_SOURCE_DIR}/new_include)
