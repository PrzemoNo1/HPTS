#include <iostream>
#include "core/thread_pool.hpp"

#include <chrono>
#include <thread>

std::chrono::milliseconds ONE_SEC(1000);
std::chrono::milliseconds TWO_SEC(2000);
std::chrono::milliseconds THREE_SEC(3000);


int main(int argc, char* argv[])
{
    //std::this_thread::sleep_for(timespan);
    core::ThreadPool tp(2);
    tp.submit([](){
        std::cout << "Starting task A submitted from main" << std::endl;
        std::this_thread::sleep_for(THREE_SEC);
        std::cout << "Finished task A" << std::endl;
    });
    tp.submit([](){ std::cout << "Task B submitted from main" << std::endl; });
    tp.submit([](){ std::cout << "Task C submitted from main" << std::endl; });
    return 0;
}