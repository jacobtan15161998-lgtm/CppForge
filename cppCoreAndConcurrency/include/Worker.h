//
// Created by JacobTan on 2026/10/19.
//

#pragma once
#include <atomic>
#include "BlockedQueue.h"

void producer(BlockedQueue& queue, int id, int count);
void consumer(BlockedQueue& queue, int id, std::atomic<int>& total);
