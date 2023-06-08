//
// Created by zeliboba on 6/8/23.
//

#ifndef DININGPHILOSOPHERS_PLATE_H
#define DININGPHILOSOPHERS_PLATE_H


#include <string>

class Plate {
public:
    [[nodiscard]] bool IsBeingConsumed() const {
        return isBeingConsumed_;
    }

    void Access() {
        isBeingConsumed_ = true;
    }

    void Release() {
        isBeingConsumed_ = false;
    }

    [[nodiscard]] std::string ToString() const {
        if (IsBeingConsumed())
            return "is being consumed";
        return "is not being consumed";
    }
private:
    bool isBeingConsumed_ = false;
};


#endif //DININGPHILOSOPHERS_PLATE_H
