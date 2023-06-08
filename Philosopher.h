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
        table_(table),
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
        GetPlate().Access();
        meals_count_++;
        state_ = State::EATING;
        WaitForRandomTime();
        GetPlate().Release();
    }

    void WaitForRandomTime() {
        int milliseconds = distribution_(generator_);
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    Plate& GetPlate() {
        return table_.TakePlate(seat_number_);
    }

    Fork& GetLeftFork() {
        return table_.TakeLeftFork(seat_number_);
    }

    Fork& GetRightFork() {
        return table_.TakeRightFork(seat_number_);
    }

    Waiter& GetWaiter() {
        return table_.GetWaiter();
    }

    int seat_number_;
    Table& table_;
    State state_ = State::HUNGRY;

    int meals_count_ = 0;
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> distribution_{0, 15};
};

#endif //DININGPHILOSOPHERS_PHILOSOPHER_H
