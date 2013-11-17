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
