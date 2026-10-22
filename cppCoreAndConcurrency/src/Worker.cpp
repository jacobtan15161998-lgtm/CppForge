//
// Created by JacobTan on 2026/10/19.
//
#include <iostream>
#include <string>
#include <atomic>
#include "BlockedQueue.h"

void producer(BlockedQueue &queue, int id, int count) {
    for (int i = 0; i < count; i++) {
        std::string msg = "Thread" + std::to_string(id) + "-msg" + std::to_string(i);
        queue.push(msg);
    }
}
void consumer(BlockedQueue &queue, int id, std::atomic<int>& total ) {
    std::string msg;
    int local_count = 0;
    while (queue.pop(msg)) {
         local_count++;// 返回 false 表示该退出
        if (local_count % 500 == 0) {
            std::cout << "Thread" << id << " 已消费 " << local_count
                      << " 条，全局总计 " << total.load() << std::endl;
        }
        total++;  // atomic 的 ++ 是原子操作
    }
}
