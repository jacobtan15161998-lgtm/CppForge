cmake_minimum_required(VERSION 4.2)
project(cppCoreAndConcurrency)

set(CMAKE_CXX_STANDARD 17)

add_executable(cppCoreAndConcurrency src/main.cpp
        src/BlockedQueue.cpp
        src/Worker.cpp)

# 指定头文件搜索路径 (-Iinclude)
target_include_directories(cppCoreAndConcurrency PRIVATE include)
