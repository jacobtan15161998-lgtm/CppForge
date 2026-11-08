cmake_minimum_required(VERSION 4.2)
project(rt_servo_core CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 开启调试信息，方便你用 GDB 调试
set(CMAKE_BUILD_TYPE Debug)

# 生成静态库
add_library(rt_servo STATIC src/rt_controller.cpp)

# 指定头文件搜索路径 (-Iinclude)
target_include_directories(rt_servo PUBLIC include)

# 生成可执行文件
add_executable(main_test src/main.cpp)
target_link_libraries(main_test PRIVATE rt_servo pthread)


