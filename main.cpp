#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include "Philosopher.h"
#include "Table.h"
#include "Util/WriterStarveFreeSharedMutex.h"

int main() {
    // create a table with 5 seats
    Table table(5);

    // create philosophers
    std::vector<Philosopher> philosophers;
    for (int i = 0; i < 5; ++i) {
        philosophers.emplace_back(table, i);
    }

    // to avoid race conditions during printing
    WriterStarveFreeSharedMutex print_mutex;

    // launch threads
    std::vector<std::thread> threads;
    for (auto& philosopher : philosophers) {
        threads.emplace_back([&philosopher, &print_mutex]() {
            auto start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < 10) {
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

    // output state every second
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < 10) {
        {
            std::unique_lock<WriterStarveFreeSharedMutex> lock(print_mutex);
            std::cout << table.ToString() << std::endl;
            for (const auto& philosopher : philosophers) {
                std::cout << philosopher.ToString() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // join threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
