#include <iostream>
#include "core/thread_pool.hpp"

int main(int argc, char* argv[])
{
    core::ThreadPool tp(-1);
    tp.submit([](){ std::cout << "Task submitted from main" << std::endl; });
    return 0;
}