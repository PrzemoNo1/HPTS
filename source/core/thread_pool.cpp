#include "thread_pool.hpp"

#include <thread>

#include <iostream>

namespace core
{

class ThreadPool::Impl
{
public:
    Impl(int threads)
    {
        m_thread = std::thread(std::bind(Impl::takeTask, this));
    }

    ~Impl()
    {
        m_thread.join();
    }

private:
    void takeTask()
    {
        std::cout << "Taking task" << std::endl;
    }

    std::thread m_thread;
};

ThreadPool::ThreadPool(int threads)
{
    m_impl = new Impl(threads);
}

ThreadPool::~ThreadPool()
{
    delete m_impl;
}

void ThreadPool::submit(Task task)
{

}

} // namespace core