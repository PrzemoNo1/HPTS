#include "thread_pool.hpp"

#include <thread>
#include <queue>
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

    void submit(Task task)
    {
        m_tasks.emplace(task);
    }

private:
    void takeTask()
    {
        std::cout << "Taking task" << std::endl;

        Task task = m_tasks.front();
        m_tasks.pop();
        task();
    }

    std::thread m_thread;
    std::queue<Task> m_tasks;
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
    m_impl->submit(std::move(task));
}

} // namespace core