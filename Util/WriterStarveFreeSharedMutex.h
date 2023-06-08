//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H
#define DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H


#include <thread>
#include <shared_mutex>
#include <mutex>
#include <condition_variable>

// Inconsistent naming with the rest of the project to be able to use std::shared_lock and std::unique_lock
class WriterStarveFreeSharedMutex {
    std::shared_mutex shared_mutex_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> write_lock_pending_{false};

public:
    void lock() {
        std::unique_lock<std::mutex> lock(mtx_);
        write_lock_pending_.store(true);
        shared_mutex_.lock();
        write_lock_pending_.store(false);
        cv_.notify_all();
    }

    void unlock() {
        shared_mutex_.unlock();
    }

    void lock_shared() {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this]() { return !write_lock_pending_.load(); });
        }

        shared_mutex_.lock_shared();
    }

    void unlock_shared() {
        shared_mutex_.unlock_shared();
    }
};


#endif //DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H
