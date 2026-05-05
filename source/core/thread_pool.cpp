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
        unsigned int maxThreads = std::thread::hardware_concurrency();
        if (maxThreads == 0) maxThreads = 2;
        if (threads > maxThreads)
        {
            std::cout << "Warning: Changing threads number from " << threads << " to " << maxThreads << std::endl;
            threads = maxThreads;
        }
        m_threads.reserve(threads);
        for (int i = 0; i < threads; ++i)
        {
            m_threads.emplace_back(&Impl::takeTask, this);
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
        while (m_waitings < 3)
        {
            m_mutex.lock();
            if (m_tasks.empty())
            {
                m_mutex.unlock();
                std::cout << std::this_thread::get_id() << ": Go to sleep" << std::endl;
                std::chrono::milliseconds sec(1000);
                std::this_thread::sleep_for(sec);
                ++m_waitings;
                continue;
            }
            m_waitings = 0;
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
    uint8_t m_waitings{0};
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