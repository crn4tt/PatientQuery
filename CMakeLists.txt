cmake_minimum_required(VERSION 3.10)

project(Patient_Queue)

set(CMAKE_CXX_STANDARD 17)

include_directories(./includes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

set(SOURCES
    ./DataBaseWorker.cpp
    ./Patient.cpp
    ./Visit.cpp
    ./PatientQueue.cpp
    ./Doctor.cpp
    ./main.cpp
)

add_executable(Patient_Queue ${SOURCES})
TARGET_LINK_LIBRARIES(Patient_Queue  ${PQXX_LIB} ${PQ_LIB})
