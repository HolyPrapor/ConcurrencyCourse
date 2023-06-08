#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include "Philosopher.h"
#include "Table.h"
#include "Util/WriterStarveFreeSharedMutex.h"

const int NUM_SEATS = 500;
const int RUN_SECONDS_DURATION = 10;
const int PRINT_SECONDS_FREQUENCY = 1;

int main() {
    Table table(NUM_SEATS);

    std::vector<Philosopher> philosophers;
    for (int i = 0; i < NUM_SEATS; ++i) {
        philosophers.emplace_back(table, i);
    }

    // to avoid race conditions during printing
    WriterStarveFreeSharedMutex print_mutex;

    std::vector<std::thread> threads;
    for (auto& philosopher : philosophers) {
        threads.emplace_back([&philosopher, &print_mutex]() {
            auto start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < RUN_SECONDS_DURATION) {
                {
                    std::shared_lock<WriterStarveFreeSharedMutex> lock(print_mutex);
                    philosopher.Eat();
                }
                {
                    std::shared_lock<WriterStarveFreeSharedMutex> lock(print_mutex);
                    philosopher.Think();
                }
            }
        });
    }

    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < RUN_SECONDS_DURATION) {
        {
            std::unique_lock<WriterStarveFreeSharedMutex> lock(print_mutex);
            // was used for debugging, makes no sense now
            // std::cout << table.ToString() << std::endl;
            for (const auto& philosopher : philosophers) {
                std::cout << philosopher.ToString() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(PRINT_SECONDS_FREQUENCY));
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
