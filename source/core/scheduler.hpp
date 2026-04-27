#include <functional>

using Task = std::function<void()>;

namespace core
{

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();
    void submit(Task task);

private:
    class Impl;
    Impl* m_impl;
};

} // namespace core