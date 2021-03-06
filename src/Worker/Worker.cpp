/*
Copyright 2013, 2014 Kevin Robert Stravers

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


// Headers
#include "Worker/Worker.hpp"
#include <iostream>


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
