////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Worker/Worker.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Worker::Worker()
    :
        m_thread(&Worker::work, this)
    {}

    ////////////////////////////////////////////////////////////
    Worker::~Worker()
    {
        m_work = nullptr;
        m_work_available.notify();
    }

    ////////////////////////////////////////////////////////////
    void Worker::work()
    {
        top:
            m_work_available.wait();
            std::function<void()> function = m_work;
            if (function)
                function();
            else
                return;
        goto top;
    }

} // Namespace ttl
