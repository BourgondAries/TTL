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


#ifndef WORKER_HPP_INCLUDED
#define WORKER_HPP_INCLUDED

// Headers
#include <TTL/Flare/Flare.hpp>
#include <TTL/JoinThread/JoinThread.hpp>
#include <functional>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief A thread waiting for a task
    ///
    /// This class is mainly used internally by the threadpool.
    /// This Worker thread is easily extended, as one could pass
    /// any function into issueWork. For example; in ThreadPool
    /// this class is used, and a function that encapsulates
    /// both thread management and the actual work itself will
    /// be sent into the Worker from within ThreadPool.
    ///
    ////////////////////////////////////////////////////////////
    class Worker
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ////////////////////////////////////////////////////////////
        Worker();

        ////////////////////////////////////////////////////////////
        /// \brief Constructor that runs a function at once
        ////////////////////////////////////////////////////////////
        template <typename T>
        Worker(T function)
        :
            m_work_available(true),
            m_work(function),
            m_thread(&Worker::work, this)
        {}

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ////////////////////////////////////////////////////////////
        ~Worker();

        ////////////////////////////////////////////////////////////
        /// \brief Sets the work to be done and activates the worker
        ///
        /// If this function is called before the worker can wake up
        /// then behaviour is well-defined. The worker will work with
        /// the latest update.
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWork(T function)
        {
            m_work = function;
            m_work_available.notify();
        }

    private:

        ////////////////////////////////////////////////////////////
        /// \brief Internal function used to wait for work
        ////////////////////////////////////////////////////////////
        void work();

        ttl::Flare m_work_available; ///< Notified when work is added
        std::function<void()> m_work; ///< The work to be done
        ttl::JoinThread m_thread; ///< The thread that works

    };

} // Namespace ttl

#endif // WORKER_HPP_INCLUDED
