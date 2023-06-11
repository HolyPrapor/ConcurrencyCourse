//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_WAITER_H
#define DININGPHILOSOPHERS_WAITER_H


#include <queue>
#include <mutex>
#include <condition_variable>

class Waiter {
public:
    void AskPermissionToEat(int seat_number) {
        std::unique_lock<std::mutex> lock(mtx_);
        queue_.push(seat_number);
        cv_.wait(lock, [&]() { return queue_.front() == seat_number; });
    }

    void FinishedEating(int seat_number) {
        std::unique_lock<std::mutex> lock(mtx_);
        if (queue_.front() == seat_number) {
            queue_.pop();
        }
        cv_.notify_all();
    }

private:
    std::queue<int> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
};


#endif //DININGPHILOSOPHERS_WAITER_H
