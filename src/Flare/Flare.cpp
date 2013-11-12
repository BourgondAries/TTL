/*
Copyright 2013 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/


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
