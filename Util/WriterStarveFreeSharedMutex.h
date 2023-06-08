//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H
#define DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H


#include <thread>
#include <shared_mutex>

// Inconsistent naming with the rest of the project to be able to use std::shared_lock and std::unique_lock
class WriterStarveFreeSharedMutex {
    std::shared_mutex mutex_;
    std::atomic<bool> write_lock_pending_{false};

public:
    void lock() {
        write_lock_pending_.store(true);
        mutex_.lock();
        write_lock_pending_.store(false);
    }

    void unlock() {
        mutex_.unlock();
    }

    void lock_shared() {
        while (write_lock_pending_.load()) {
            std::this_thread::yield();
        }
        mutex_.lock_shared();
    }

    void unlock_shared() {
        mutex_.unlock_shared();
    }
};


#endif //DININGPHILOSOPHERS_WRITERSTARVEFREESHAREDMUTEX_H
