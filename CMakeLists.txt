cmake_minimum_required(VERSION 3.10)
project(Patient_Queue)

find_library(PQXX_LIB libpqxx)

include_directories(/usr/include/pqxx)

add_executable(Patient_Queue main.cpp)

target_link_libraries(Patient_Queue PRIVATE libpqxx)


