cmake_minimum_required(VERSION 3.10)
project(lan-scanner CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lan-scanner src/main.cpp)

if(WIN32)
    target_link_libraries(lan-scanner PRIVATE ws2_32)
endif() 
