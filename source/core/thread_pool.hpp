#include <functional>
#include <memory>
#include <future>

using Task = std::function<int()>;

namespace core
{

class ThreadPool
{
public:
    ThreadPool(uint8_t threads);
    ~ThreadPool();
    std::future<int> submit(Task task);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace core