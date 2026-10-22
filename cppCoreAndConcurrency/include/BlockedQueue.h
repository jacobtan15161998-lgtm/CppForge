//
// Created by JacobTan on 2026/10/19.
//

#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>


class BlockedQueue {
public:
   // explicit：防隐式转换，防止 BlockedQueue q = 10; 这种诡异写法。
   explicit BlockedQueue(int max_size);
   void push(const std::string& message);
   bool pop(std::string& msg);
   void set_done();
private:
   int max_size;
   std::queue<std::string> queue_;
   std::condition_variable is_not_empty;
   std::condition_variable is_not_full;
   std::mutex mutex;
   bool done = false;
};
