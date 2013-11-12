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


#ifndef BATCHWORKER_HPP_INCLUDED
#define BATCHWORKER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <thread>
#include <cstddef>
#include <atomic>
#include <vector>
#include "Flare/Flare.hpp"
#include "Worker/Worker.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Multiple workers that process a batch
    ///
    ////////////////////////////////////////////////////////////
    class BatchWorker
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        ////////////////////////////////////////////////////////////
        BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Set the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        void setWorkerCount(const std::size_t workers);

        ////////////////////////////////////////////////////////////
        /// \brief Get the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        std::size_t getWorkerCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Send work to a specified worker
        ///
        /// Work must be within the range [0, getWorkerCount() - 1]
        /// No checks are performed to assure this, it is up to you.
        ///
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWork(T t, const std::size_t thread)
        {
            ++m_actively_working;
            m_thread_pool[thread]->issueWork
            (
                [t, this]()
                {
                    t();
                    if (m_actively_working.fetch_sub(1) == 1)
                        m_threads_done.notify();
                }
            );
        }

        ////////////////////////////////////////////////////////////
        /// \brief Wait for all workers to finish their current task.
        ///
        ////////////////////////////////////////////////////////////
        void wait();

    private:

        std::vector<std::unique_ptr<Worker>> m_thread_pool; ///< Collection of workers
        std::atomic<std::size_t> m_actively_working; ///< Counter of actively working workers
        ttl::Flare m_threads_done; ///< Notified when all threads have finished.
    };

} // Namespace ttl

#endif // BATCHWORKER_HPP_INCLUDED
