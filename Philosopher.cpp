//
// Created by zeliboba on 6/8/23.
//

#include "Philosopher.h"

void Philosopher::AcquireForks() {
    GetWaiter().AskPermissionToEat(GetSeatNumber());

    GetLeftFork().Take(GetSeatNumber());
    GetRightFork().Take(GetSeatNumber());
}

void Philosopher::ReleaseForks() {
    GetLeftFork().Put();
    GetRightFork().Put();

    GetWaiter().FinishedEating(GetSeatNumber());
}
