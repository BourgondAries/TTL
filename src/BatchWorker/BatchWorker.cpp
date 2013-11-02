////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "BatchWorker/BatchWorker.hpp"
#include "Debug/Debug.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    BatchWorker::BatchWorker()
    :
        m_actively_working(0)
    {
    }

    ////////////////////////////////////////////////////////////
    BatchWorker::~BatchWorker()
    {}

    ////////////////////////////////////////////////////////////
    void BatchWorker::setWorkerCount(const std::size_t workers)
    {
        if (m_thread_pool.size() > workers)
        {
            m_thread_pool.erase(m_thread_pool.begin() + workers, m_thread_pool.end());
            return;
        }
        for (; m_thread_pool.size() < workers;)
        {
            m_thread_pool.emplace_back(new Worker);
        }
    }

    ////////////////////////////////////////////////////////////
    std::size_t BatchWorker::getWorkerCount() const
    {
        return m_thread_pool.size();
    }

    ////////////////////////////////////////////////////////////
    void BatchWorker::wait()
    {
        m_threads_done.wait();
    }


} // Namespace ttl
