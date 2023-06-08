//
// Created by zeliboba on 6/8/23.
//

#include "Philosopher.h"

static void Acquire(Philosopher* philosopher, Fork& first, Fork& second) {
    if (first.GetForkNumber() > second.GetForkNumber())
        Acquire(philosopher, second, first);

    first.Take(philosopher->GetSeatNumber());
    second.Take(philosopher->GetSeatNumber());
}

void Philosopher::AcquireForks() {
    bool shouldSwap = this->left_fork_.GetForkNumber() < this->right_fork_.GetForkNumber();
    auto &lower = shouldSwap ? this->left_fork_ : this->right_fork_;
    auto &higher = shouldSwap ? this->right_fork_ : this->left_fork_;

    lower.Take(this->GetSeatNumber());
    higher.Take(this->GetSeatNumber());
}

void Philosopher::ReleaseForks() {
    bool shouldSwap = this->left_fork_.GetForkNumber() < this->right_fork_.GetForkNumber();
    auto &lower = shouldSwap ? this->left_fork_ : this->right_fork_;
    auto &higher = shouldSwap ? this->right_fork_ : this->left_fork_;

    lower.Put();
    higher.Put();
}
