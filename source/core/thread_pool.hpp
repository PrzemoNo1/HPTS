#include <functional>
#include <memory>

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
    std::unique_ptr<Impl> m_impl;
};

} // namespace core