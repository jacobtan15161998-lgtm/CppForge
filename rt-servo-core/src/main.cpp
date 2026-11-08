#include "rt_controller.h"
#include <iostream>
#include <chrono>

int main() {
    RTController controller;

    // 定义这 1ms 里面要干的事情
    // 现在先什么都不干，或者打印一下周期号
    auto my_task = [](int64_t cycle) {
        // 这里后续可以换成：读总线、更新状态机、写 PDO
        // 现在只为了证明循环在转
        if (cycle % 1000 == 0) {
            std::cout << "--- 已经跑了 " << cycle << " 个周期 ---" << std::endl;
        }
    };

    std::cout << "启动 RTController (记得用 sudo 运行)..." << std::endl;
    
    // 启动实时引擎
    if (!controller.start(my_task)) {
        std::cerr << "启动失败" << std::endl;
        return -1;
    }

    // 等待它自己跑完 5000 次（5秒）
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    // 获取最终的最大 Jitter
    std::cout << "外部读取最大 Jitter: " << controller.get_max_jitter_ns() / 1000.0 << " µs" << std::endl;

    controller.stop();
    return 0;
}