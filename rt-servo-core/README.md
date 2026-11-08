# RT-Servo-Core (实时伺服控制核心)

## 📌 项目背景
在工业自动化（如半导体设备、机械臂）中，上层软件与底层伺服驱动器的通信必须具备极高的确定性。传统的 `sleep` 和带锁阻塞会导致控制周期抖动（Jitter）甚至丢帧。本项目基于 C++17 和 Linux，构建了一个具备微秒级调度精度、零锁竞争、且符合 CiA 402 工业标准的底层控制中间件。

## 🛠️ 核心技术栈
- **语言/系统**: C++17, RT-Linux (PREEMPT_RT), CMake
- **实时调度**: `clock_nanosleep` (绝对时间), `SCHED_FIFO`, `mlockall`
- **并发模型**: Lock-free SPSC Ring Buffer (基于 `std::atomic` 实现)
- **通信协议**: CiA 402 伺服状态机 (SOEM / Modbus TCP 虚拟驱动)

## 🚀 核心功能与实现
1. **硬实时调度与 Jitter 压制**：使用绝对时间定时器与最高优先级抢占，实现了稳定在 1ms 的控制周期；在系统高负载（`stress-ng` 压测）下，通过 `cyclictest` 验证调度精度。
2. **无锁数据解耦**：手写 `SPSCRingBuffer`，通过缓存行对齐（`alignas(64)`）与内存序（`acquire/release`）优化，实现 1ms 实时线程与后台日志线程的零锁通信，杜绝优先级反转。
3. **CiA 402 伺服状态机**：手写标准 `switch-case` 状态机，实现从 `Switch On Disabled` 到 `Operation Enabled` 的自动化流转，确保底层电机的安全使能。

## 📊 验收与演示 (附上你的截图)
- **Jitter 压测数据**: (插入你 WSL 下或原生 Linux 下 `cyclictest` 的 Max Latencies 截图)
- **状态机流转日志**: (插入终端打印的 `[FSM] State changed to: READY` 等日志截图)

## 🧠 踩坑与思考
- **WSL 与 RT-Linux 的边界**：开发初期在 WSL2 中验证了通信与 FSM 逻辑，但在压测中发现 WSL 的虚拟化层无法保证硬实时（Jitter > 40µs）。这让我深刻理解了裸机物理机与虚拟化环境的调度差异。
- **锁的代价**：在调试生产者-消费者时，通过 GDB 抓取到 `mutex` 导致的高优先级线程阻塞（优先级反转），这成为了我编写无锁队列的最大动力。