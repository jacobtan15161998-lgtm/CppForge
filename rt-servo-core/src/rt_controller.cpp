#include "rt_controller.h"
#include <iostream>
#include <sched.h>      // 提供 SCHED_FIFO
#include <time.h>       // 提供 clock_nanosleep, CLOCK_MONOTONIC
#include <sys/mman.h>   // 提供 mlockall
#include <cstring>
#include <cmath>

RTController::RTController() {}

RTController::~RTController() {
    stop();
}

bool RTController::start(CycleCallback task) {
    if (running_) return false;
    running_ = true;
    rt_thread_ = std::thread(&RTController::run_loop, this, task);
    return true;
}
void RTController::stop() {
    running_ = false;
    if (rt_thread_.joinable()) {
        rt_thread_.join();
    }
}
void RTController::run_loop(CycleCallback task) {
    // ==========================================
    // 1. 设置实时调度策略：SCHED_FIFO，优先级 99 实时优先级调度1-99 99为最高优先级
    // ==========================================
    struct sched_param param;
    param.sched_priority = 99;
    // pid 传 0 代表当前调用线程（也就是这个实时线程）
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        std::cerr << "[RT-Error] 设置 SCHED_FIFO 失败，请确保使用 sudo 运行！" << std::endl;
    } else {
        std::cout << "[RT-Info] SCHED_FIFO 设置成功，优先级: " << param.sched_priority << std::endl;
    }

    // ==========================================
    // 2. 锁定内存，防止运行中出现缺页中断导致延迟
    // ==========================================
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        std::cerr << "[RT-Error] 内存锁定失败！" << std::endl;
    } else {
        std::cout << "[RT-Info] mlockall 成功，内存已锁定。" << std::endl;
    }

    // ==========================================
    // 3. 获取绝对时间基准，准备进入 1ms 循环
    // ==========================================
    struct timespec next_wakeup;
    clock_gettime(CLOCK_MONOTONIC, &next_wakeup);

    std::cout << "[RT-Info] 1ms 实时循环启动！" << std::endl;

    int64_t cycle_count = 0;
    int64_t max_jitter = 0;

    while (running_) {
        // 【业务执行区】：这里执行你传入的函数（今天我们只数数）
        if (task) {
            task(cycle_count);
        }
        // ==========================================
        // 4. 计算下一个周期的绝对时间 (加 1ms)
        // ==========================================
        next_wakeup.tv_nsec += 1000000; // 1,000,000 纳秒 = 1ms

        next_wakeup.tv_nsec += 1000000; // 1,000,000 纳秒 = 1ms
        if (next_wakeup.tv_nsec >= 1000000000) {
            next_wakeup.tv_sec++;
            next_wakeup.tv_nsec -= 1000000000;
        }

        // ==========================================
        // 5. 使用 clock_nanosleep 睡眠到绝对时间 (TIMER_ABSTIME)
        // ==========================================
        int ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_wakeup, NULL);
        // ==========================================
        // 6. 醒来后，立即计算 Jitter (实际醒来时间 - 理论时间)
        // ==========================================
        if (ret == 0) {
            struct timespec actual_wakeup;
            clock_gettime(CLOCK_MONOTONIC, &actual_wakeup);

            // 计算延迟（纳秒）
            int64_t jitter_ns = (actual_wakeup.tv_sec - next_wakeup.tv_sec) * 1000000000LL +
                                (actual_wakeup.tv_nsec - next_wakeup.tv_nsec);

            // 记录最大值
            if (jitter_ns > max_jitter) {
                max_jitter = jitter_ns;
                max_jitter_ns_ = jitter_ns; // 供外部读取
            }
        } else if (ret != EINTR) {
            std::cerr << "[RT-Error] clock_nanosleep 异常" << std::endl;
            break;
        }

        cycle_count++;

        // 测试跑 5000 次（5秒）自动退出，方便你测试
        if (cycle_count >= 5000) {
            std::cout << "[RT-Info] 已完成 5000 个周期 (5秒)。最大 Jitter: " << max_jitter << " ns (" << max_jitter / 1000.0 << " µs)" << std::endl;
            break;
        }
    }
    running_ = false;


}


