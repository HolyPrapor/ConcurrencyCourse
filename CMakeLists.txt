cmake_minimum_required(VERSION 3.25)
project(DiningPhilosophers)

set(CMAKE_CXX_STANDARD 20)

add_executable(DiningPhilosophers main.cpp Fork.cpp Fork.h Table.cpp Table.h Plate.cpp Plate.h Philosopher.cpp Philosopher.h Util/WriterStarveFreeSharedMutex.cpp Util/WriterStarveFreeSharedMutex.h)
