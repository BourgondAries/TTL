////////////////////////////////////////////////////////////
//
// TTL - Tea Tank Library
// Copyright (C) 2013 Kevin R. Stravers (macocio@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Flare/Flare.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Flare::Flare(bool skip_on_first_wait)
    :
    m_notifications(skip_on_first_wait)
    {}

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
