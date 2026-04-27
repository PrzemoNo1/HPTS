#include "thread_pool.hpp"

namespace core
{

class ThreadPool::Impl
{

};

ThreadPool::ThreadPool(int threads)
{
    m_impl = new Impl;
}

ThreadPool::~ThreadPool()
{
    delete m_impl;
}

void ThreadPool::submit(Task task)
{

}

} // namespace core