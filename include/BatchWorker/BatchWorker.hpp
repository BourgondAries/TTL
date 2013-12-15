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

// Headers
#include <thread>
#include <cstddef>
#include <atomic>
#include <vector>
#include <TTL/Flare/Flare.hpp>
#include <TTL/Worker/Worker.hpp>


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

        ////////////////////////////////////////////////////////////
        /// \brief A simple interface for performing parallel fors
        /// over standard-compliant iteratable containers.
        ///
        /// Applies a specified function to [begin, end) elements.
        ///
        /// \param begin The start iterator.
        /// \param end The last iterator.
        /// \param function the function that accepts a
        /// dereferenced iterator as its parameter (which is a
        /// reference).
        ///
        ////////////////////////////////////////////////////////////
        template <typename ITERATOR, typename FUNCTION>
        void fer(ITERATOR begin, ITERATOR end, FUNCTION fun)
        {
            std::size_t thread_pool_size(m_thread_pool.size());
            for (std::size_t i(0); i < thread_pool_size; ++i)
            {
                this->issueWork
                (
                    [thread_pool_size, i, begin, end, fun]() -> void
                    {
                        ITERATOR current(begin);
                        if (std::distance(current, end) > i)
                        {
                            std::advance(current, i);
                            fun(*current);
                            while (std::distance(current, end) > thread_pool_size)
                            {
                                std::advance(current, thread_pool_size);
                                fun(*current);
                            }
                        }
                    },
                    i
                );
            }
            this->wait();
        }

        ////////////////////////////////////////////////////////////
        /// \brief A simple interface for performing parallel fors
        /// over standard-compliant iteratable containers.
        ///
        /// \param container The container to iterate over
        /// Requires CONTAINER::begin() and CONTAINER::end()
        /// which have an operator++(int) methods.
        /// \param function the function that accepts a
        /// dereferenced iterator as its parameter (which is a
        /// reference).
        ///
        ////////////////////////////////////////////////////////////
        template <typename CONTAINER, typename FUNCTION>
        void fer(CONTAINER container, FUNCTION function)
        {
            fer(container.begin(), container.end(), function);
        }

    private:

        std::vector<std::unique_ptr<Worker>> m_thread_pool; ///< Collection of workers
        std::atomic<std::size_t> m_actively_working; ///< Counter of actively working workers
        ttl::Flare m_threads_done; ///< Notified when all threads have finished.
    };

} // Namespace ttl

#endif // BATCHWORKER_HPP_INCLUDED
