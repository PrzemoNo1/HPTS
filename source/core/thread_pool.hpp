#include <functional>
#include <memory>
#include <future>
#include <vector>
#include <queue>
#include <iostream>


template<typename T>
using Task = std::function<T()>;

namespace core
{
using InnerTask = std::function<void()>;

class ThreadPool
{
public:
    ThreadPool(uint8_t threads);
    ~ThreadPool();

    template<typename T>
    std::future<T> submit(Task<T> task)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::shared_ptr<std::packaged_task<T()>> pt =
            std::make_shared<std::packaged_task<T()>>(task);
        std::future<T> f = pt->get_future();

        m_tasks.emplace([pt] () {(*pt)();});
        m_cv.notify_one();
        return f;
    }

private:
    void takeTask();

    std::vector<std::thread> m_threads;
    std::queue<InnerTask> m_tasks;
    std::mutex m_mutex;

    std::condition_variable m_cv;
    bool m_stop = false;
};

} // namespace core