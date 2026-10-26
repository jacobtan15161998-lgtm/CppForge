//
// Created by JacobTan on 2026/10/23.
// （1） mkdir build && cd build 创建build目录并切换至build目录
// （2） cmake -DCMAKE_BUILD_TYPE=Debug .. 在上级目录调用cmake打开debug模式
// （3） make 根据上一步 CMake 文件执行编译等后续操作
// （4） gdb ./crash 启动 GDB（GNU 调试器），并加载当前目录下名为 crash 的可执行文件
//
#include <iostream>

class Motor {
public:
    int speed;
};
int main() {
    std::cout << "程序开始运行..." << std::endl;

    Motor* myMotor = nullptr; // 故意搞一个空指针

    // 在 C++ 里，这就是直接访问非法内存，触发段错误 (Segmentation fault)
    myMotor->speed = 100;

    std::cout << "这行代码永远不会被执行" << std::endl;
    return 0;
}
