//
// Created by JacobTan on 2026/11/8.
//

#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include <cstdint>
class RTController {
    public:
    // 定义一个“周期任务”的函数类型。
    // 后续我们可以把“读总线”、“跑状态机”等逻辑作为参数传进来。
    // 参数 int64_t 代表当前这个周期的序号（第 1 个 1ms，第 2 个 1ms...）
        using CycleCallback = std::function<void(int64_t)>;

        RTController();
        ~RTController();

    // 启动实时循环，传入需要周期性执行的任务
        bool start(CycleCallback task);

    // 停止循环
        void stop();
    // 获取最大抖动时间（纳秒）
        int64_t get_max_jitter_ns() const { return max_jitter_ns_; }

    private:
    // 真正的实时控制循环体（跑在独立线程里）
        void run_loop(CycleCallback task);

        std::atomic<bool> running_{false};
        std::thread rt_thread_;

    // 记录最大抖动
        std::atomic<int64_t> max_jitter_ns_{0};

};
