#include "thread_pool.hpp"

#include <thread>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

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

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_cv.notify_all();
  
        for (auto& thread : m_threads)
        {
            thread.join();
        }
    }

    void submit(Task task)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.emplace(std::move(task));
        m_cv.notify_one();
    }

private:
    void takeTask()
    {
        while (true)
        {
            std::unique_lock<std::mutex> locker(m_mutex);//.lock();
            m_cv.wait(locker, [&](){return !m_tasks.empty() || m_stop; });

            if (m_stop && m_tasks.empty())
                return;

            std::cout << std::this_thread::get_id() << ": Taking task" << std::endl;

            Task task = std::move(m_tasks.front());
            m_tasks.pop();
            locker.unlock();
            task();
        }
    }

    std::vector<std::thread> m_threads;
    std::queue<Task> m_tasks;
    std::mutex m_mutex;

    std::condition_variable m_cv;
    bool m_stop = false;
};

ThreadPool::ThreadPool(uint8_t threads)
{
    m_impl = std::make_unique<Impl>(threads);
}

ThreadPool::~ThreadPool() = default;

void ThreadPool::submit(Task task)
{
    m_impl->submit(std::move(task));
}

} // namespace core