//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_PHILOSOPHER_H
#define DININGPHILOSOPHERS_PHILOSOPHER_H


#include <thread>
#include <random>
#include "Table.h"

enum State {
    HUNGRY = 0,
    EATING = 1,
    THINKING = 2
};

class Philosopher {
public:
    explicit Philosopher(Table& table, int seat_number) :
    Philosopher(
            table.TakePlate(seat_number),
            table.TakeLeftFork(seat_number),
            table.TakeRightFork(seat_number),
            seat_number
    ) {}

    explicit Philosopher(Plate& plate, Fork& left_fork, Fork& right_fork, int seat_number) :
        plate_(plate),
        left_fork_(left_fork),
        right_fork_(right_fork),
        seat_number_(seat_number) {}

    void Eat() {
        AcquireForks();
        EatWithForks();
        ReleaseForks();
    }

    void Think() {
        state_ = State::THINKING;
        WaitForRandomTime();
        state_ = State::HUNGRY;
    }

    [[nodiscard]] int GetSeatNumber() const {
        return seat_number_;
    }

    [[nodiscard]] int GetMealsCount() const {
        return meals_count_;
    }

    [[nodiscard]] std::string ToString() const {
        std::stringstream ss;

        ss << "Philosopher #" << seat_number_ << "\n";
        ss << "- State: " << state_ << "\n";
        ss << "- Consumed " << GetMealsCount() << " meals\n";

        return ss.str();
    }

private:
    void AcquireForks();
    void ReleaseForks();

    void EatWithForks() {
        plate_.Access();
        meals_count_++;
        state_ = State::EATING;
        WaitForRandomTime();
        plate_.Release();
    }

    void WaitForRandomTime() {
        int milliseconds = distribution_(generator_);
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    int seat_number_;
    Plate& plate_;
    Fork& left_fork_;
    Fork& right_fork_;
    State state_ = State::HUNGRY;

    int meals_count_ = 0;
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> distribution_{0, 15};
};

#endif //DININGPHILOSOPHERS_PHILOSOPHER_H
