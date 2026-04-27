#include "thread_pool.hpp"

#include <thread>
#include <queue>
#include <iostream>
#include <mutex>

namespace core
{

class ThreadPool::Impl
{
public:
    Impl(uint8_t threads)
    {
        const uint8_t& maxThreads = std::thread::hardware_concurrency();
        if (threads > maxThreads)
        {
            std::cout << "Warning: Changing threads number from " << threads << " to " << maxThreads << std::endl;
            threads = maxThreads;
        }
        m_threads.reserve(threads);
        for (int i = 0; i < threads; ++i)
        {
            m_threads.emplace_back(std::bind(Impl::takeTask, this));
        }
    }

    ~Impl()
    {
        for (auto& thread : m_threads)
        {
            thread.join();
        }
    }

    void submit(Task task)
    {
        m_tasks.emplace(task);
    }

private:
    void takeTask()
    {
        m_mutex.lock();
        while (!m_tasks.empty())
        {
            std::cout << std::this_thread::get_id() << ": Taking task" << std::endl;

            Task task = m_tasks.front();
            m_tasks.pop();
            m_mutex.unlock();
            task();
        }
    }

    std::vector<std::thread> m_threads;
    std::queue<Task> m_tasks;
    std::mutex m_mutex;
};

ThreadPool::ThreadPool(uint8_t threads)
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