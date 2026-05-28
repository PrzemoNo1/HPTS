#include <iostream>
#include "core/thread_pool.hpp"

#include <chrono>
#include <thread>
#include <future>
#include <string>

std::chrono::milliseconds ONE_SEC(1000);
std::chrono::milliseconds TWO_SEC(2000);
std::chrono::milliseconds THREE_SEC(3000);


int main(int argc, char* argv[])
{
    core::ThreadPool tp(2);
    std::future<int> fu =
        tp.submit<int>([](){
        std::cout << "Starting task A submitted from main" << std::endl;
        std::this_thread::sleep_for(THREE_SEC);
        std::cout << "Finished task A" << std::endl;
        return 23;
    });
    std::future<std::string> fu2 =
        tp.submit<std::string>([](){ std::cout << "Task B submitted from main" << std::endl; return "Lore Ipsum";});
    tp.submit<void>([](){ std::cout << "Task C submitted from main" << std::endl; });

    std::this_thread::sleep_for(std::chrono::seconds(12));

    std::cout << "Future int is: " << fu.get() << std::endl;
    std::cout << "Future std::string is: " << fu2.get() << std::endl;

    return 0;
}