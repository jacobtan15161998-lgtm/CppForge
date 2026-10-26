cmake_minimum_required(VERSION 4.2)
project(testBug)

set(CMAKE_CXX_STANDARD 17)

add_executable(crash src/crash.cpp)

add_executable(leak src/leak.cpp)