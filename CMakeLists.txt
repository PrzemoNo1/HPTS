cmake_minimum_required(VERSION 3.10)

project(HPTS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HTPS
    source/main.cpp
    source/core/scheduler.cpp
    source/core/thread_pool.cpp
)
