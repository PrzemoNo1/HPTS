#include <functional>

using Task = std::function<void()>;

namespace core
{

class ThreadPool
{
public:
    ThreadPool(uint8_t threads);
    ~ThreadPool();
    void submit(Task task);

private:
    class Impl;
    Impl* m_impl;
};

} // namespace core