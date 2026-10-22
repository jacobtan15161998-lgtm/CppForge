#include <string>
#include "BlockedQueue.h"

// 加互斥锁只是解决了共享资源的竞争问题
// 条件变量的作用：空转占 CPU，生产者消费者不一直做无意义的循环检测工作条件。
// Created by JacobTan on 2026/10/19.

BlockedQueue::BlockedQueue(int max_size) : max_size(max_size) {
};

void BlockedQueue::push(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex);
    is_not_full.wait(lock,[this] { return queue_.size() < max_size || done; });
    if (done) return;
    queue_.push(message);
    is_not_empty.notify_one();
}

bool BlockedQueue::pop(std::string& msg) {
    std::unique_lock<std::mutex> lock(mutex);
    is_not_empty.wait(lock,[this] { return !queue_.empty() || done;});
    if (done && queue_.empty()) {return false;}
    msg = std::move(queue_.front());
    queue_.pop();
    is_not_full.notify_one();
    return true;
}
void BlockedQueue::set_done() {
    std::lock_guard lock(mutex);  // 没有 wait，lock_guard 够了
    done = true;
    is_not_empty.notify_all();  // 唤醒所有消费者
}

//
