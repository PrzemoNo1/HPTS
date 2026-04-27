#include <iostream>
#include "core/thread_pool.hpp"

int main(int argc, char* argv[])
{
    core::ThreadPool tp(2);
    tp.submit([](){ std::cout << "Task A submitted from main" << std::endl; });
    tp.submit([](){ std::cout << "Task B submitted from main" << std::endl; });
    tp.submit([](){ std::cout << "Task C submitted from main" << std::endl; });
    return 0;
}