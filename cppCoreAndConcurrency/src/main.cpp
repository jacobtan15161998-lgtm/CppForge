#include <atomic>
#include <thread>
#include "BlockedQueue.h"
#include "Worker.h"
#include "iostream"


int main() {
    BlockedQueue queue(10);
    std::atomic<int> total{0};

    std::thread p1(producer, std::ref(queue), 1, 1000);
    std::thread p2(producer, std::ref(queue), 2, 1000);
    std::thread c1(consumer, std::ref(queue), 1, std::ref(total));
    std::thread c2(consumer, std::ref(queue), 2, std::ref(total));

    p1.join();  // 等生产者结束
    p2.join();

    queue.set_done();  // 必须在生产者 join 之后

    c1.join();  // 等消费者结束
    c2.join();

    std::cout << "Total consumed: " << total.load() << std::endl;
    return 0;



}