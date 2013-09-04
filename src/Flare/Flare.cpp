////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Flare/Flare.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Flare::Flare(bool skip_on_first_wait)
    :
        m_notifications(skip_on_first_wait){}

    ////////////////////////////////////////////////////////////
    Flare::~Flare()
    {}

    ////////////////////////////////////////////////////////////
    void Flare::notify()
    {
        std::lock_guard<std::mutex> lock(m_mx);
        m_notifications = true;
        m_cndvar.notify_one();
    }

    ////////////////////////////////////////////////////////////
    void Flare::notify_one()
    {
        std::lock_guard<std::mutex> lock(m_mx);
        m_notifications = true;
        m_cndvar.notify_one();
    }

    ////////////////////////////////////////////////////////////
    void Flare::notify_all()
    {
        std::lock_guard<std::mutex> lock(m_mx);
        m_notifications = true;
        m_cndvar.notify_all();
    }

    ////////////////////////////////////////////////////////////
    void Flare::wait()
    {
        std::unique_lock<std::mutex> lock(m_mx);
        m_cndvar.wait(lock, [this]() -> bool {return this->m_notifications;});
        m_notifications = false;
    }

} // Namespace ttl
