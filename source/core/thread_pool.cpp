#include "thread_pool.hpp"

#include <thread>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace core
{
using InnerTask = std::function<void()>;
// TODO
// 
// returning std::future from submit
// task prioritization
// work stealing (advanced)

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

    std::future<int> submit(Task task)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::promise<int>* p = new std::promise<int>;
        std::future<int> f = p->get_future();
        InnerTask innerTask = [task, p]() {
            int result = task();
            std::cout << "Inner task after executing task" << std::endl;
            try {
                p->set_value(result);
            }
            catch (...)
            {
                std::cout << "Error after setting value";
            }
            std::cout << "Inner task after executing task2" << std::endl;
            delete p;
        };
        m_tasks.emplace(std::move(innerTask));
        m_cv.notify_one();

        return f;
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

            InnerTask task = std::move(m_tasks.front());
            m_tasks.pop();
            locker.unlock();
            task();
        }
    }

    std::vector<std::thread> m_threads;
    std::queue<InnerTask> m_tasks;
    std::mutex m_mutex;

    std::condition_variable m_cv;
    bool m_stop = false;
};

ThreadPool::ThreadPool(uint8_t threads)
{
    m_impl = std::make_unique<Impl>(threads);
}

ThreadPool::~ThreadPool() = default;

std::future<int> ThreadPool::submit(Task task)
{
    return m_impl->submit(std::move(task));
}

} // namespace core