//
// Created by JacobTan on 2026/10/23.
// valgrind --leak-check=full ./leak 使用valgrind工具进行内存泄漏检测并输出详细信息
#include <iostream>

void doSomething() {
    // 申请了 100 个整数的内存（在堆上）
    int* arr = new int[100];
    arr[0] = 1;

    // 忘记写 delete[] arr; 了！
    // 函数结束时，arr 这个指针被销毁，但堆上的 100 个整数内存变成了孤儿
}

int main() {
    for (int i = 0; i < 10; ++i) {
        doSomething();
    }
    std::cout << "程序正常结束" << std::endl;
    return 0;
}
