#include "scheduler.hpp"

namespace core
{

class Scheduler::Impl
{

};

Scheduler::Scheduler()
{
    m_impl = new Impl;
}

Scheduler::~Scheduler()
{
    delete m_impl;
}

void Scheduler::submit(Task task)
{

}

} // namespace core