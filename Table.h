//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_TABLE_H
#define DININGPHILOSOPHERS_TABLE_H

#include <mutex>
#include <vector>
#include <sstream>
#include "Fork.h"
#include "Plate.h"

class Table {
public:
    explicit Table(int num_seats) : num_seats_(num_seats), forks_(CreateForks(num_seats)), plates_(num_seats) {
    }

    Fork& TakeLeftFork(int seat) {
        CheckForUnexpected(seat);

        return forks_[seat];
    }

    Fork& TakeRightFork(int seat) {
        CheckForUnexpected(seat);

        return TakeLeftFork((seat + 1) % num_seats_);
    }

    Plate& TakePlate(int seat) {
        CheckForUnexpected(seat);

        return plates_[seat];
    }

    [[nodiscard]] std::string ToString() const {
        std::stringstream ss;

        ss << "Table with " << num_seats_ << " seats.\n";

        for (auto i = 0; i < num_seats_; i++) {
            ss << "Seat #" << (i + 1) << ":\n";
            ss << " - Fork " << forks_[i].ToString() << "\n";
            ss << " - Plate " << plates_[i].ToString() << "\n";
        }

        return ss.str();
    }

private:
    static std::vector<Fork> CreateForks(int num_seats) {
        std::vector<Fork> forks(num_seats);
        for (int i = 0; i < num_seats; i++) {
            // to avoid problems with mutex copying/moving
            forks[i].SetForkNumber_(i);
        }
        return forks;
    }

    const int num_seats_;
    std::vector<Fork> forks_;
    std::vector<Plate> plates_;

    void CheckForUnexpected(int seat) const {
        if (seat >= num_seats_)
            throw std::runtime_error("Unexpected seat");
    }
};


#endif //DININGPHILOSOPHERS_TABLE_H
