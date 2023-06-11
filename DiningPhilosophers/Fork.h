//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_FORK_H
#define DININGPHILOSOPHERS_FORK_H


#include <mutex>

class Fork {
public:
    void Take(int philosopher_seat) noexcept {
        lock_.lock();
        owner_ = philosopher_seat;
    }

    void Put() {
        lock_.unlock();
        owner_ = -1;
    }

    [[nodiscard]] std::string ToString() const {
        int currentOwner = owner_;
        if (IsLocked(currentOwner))
            return "is taken by philosopher #" + std::to_string(currentOwner);
        return "is free";
    }

    [[nodiscard]] int GetForkNumber() const {
        return fork_number_;
    }

    void SetForkNumber_(int fork_number) {
        fork_number_ = fork_number;
    }

private:
    std::mutex lock_;
    int owner_ = -1;

    [[nodiscard]] static bool IsLocked(int owner) {
        return owner != -1;
    }

    int fork_number_ = 0;
};


#endif //DININGPHILOSOPHERS_FORK_H
