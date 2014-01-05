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
#include "Ttldef/Ttldef.hpp"
#include <atomic>
#include <vector>
#include <cassert>
#include <iostream>
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
        /// \brief Constructor with worker count initialization
        ///
        ////////////////////////////////////////////////////////////
        BatchWorker(const sti worker_count);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Set the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        void setWorkerCount(const sti workers);

        ////////////////////////////////////////////////////////////
        /// \brief Get the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        sti getWorkerCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Send work to a specified worker
        ///
        /// Work must be within the range [0, getWorkerCount() - 1]
        /// No checks are performed to assure this, it is up to you.
        ///
        /// DEEP BUG:
        /// One can issue some work, which CAN finish before anything
        /// : thereby signalling the flare. This can cause a batch
        /// to partially process. Disasterous results!
        ///
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWork(const T t, const sti thread)
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

        template<typename T, typename = void>
        struct is_iterator
        {static constexpr bool value = false;};

        template<typename T>
        struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
        {static constexpr bool value = true;};

        ////////////////////////////////////////////////////////////
        /// \brief Send work to a specified worker
        ///
        /// Work must be within the range [0, getWorkerCount() - 1]
        /// No checks are performed to assure this, it is up to you.
        ///
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWorkManualIncrement(const T t, const sti thread)
        {
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

    public:

        template <typename ITERATOR, typename FUNCTION>
        void fur(ITERATOR begin, ITERATOR end, const FUNCTION fun, /*bool main_contribute = true, bool wait_for_all = true, */const sti advance = 1)
        {
            static_assert(is_iterator<ITERATOR>::value, "Arguments begin and end are not valid iterators.");
            if (begin == end)
            {
                return;
            }
            const sti thread_pool_size(m_thread_pool.size());
            m_actively_working += thread_pool_size;
            for (sti i(0); i < thread_pool_size; ++i)
            {
                this->issueWorkManualIncrement
                (
                    [i, &thread_pool_size, &begin, &end, &fun, advance]() -> void
                    {
                        sti advanceperi = i * advance;

                        ITERATOR start(begin);
                        if (std::distance(start, end) > advanceperi)
                        {
                            std::advance(start, advanceperi);
                            fun( start , i );

                            sti advancepertps = (thread_pool_size + 1) * advance;
                            while (std::distance(start, end) > advancepertps)
                            {
                                std::advance(start, advancepertps);
                                fun( start , i );
                            }
                        }
                    },
                    i
                );
            }
//            [&thread_pool_size, begin, end, &fun, advance]() -> void
            {
                sti advanceperi = (thread_pool_size) * advance;

                ITERATOR start(begin);
                if (std::distance(start, end) > advanceperi)
                {
                    std::advance(start, advanceperi);
                    fun( start, thread_pool_size );

                    sti advancepertps = (thread_pool_size + 1) * advance;
                    while (std::distance(start, end) > advancepertps)
                    {
                        std::advance(start, advancepertps);
                        fun( start, thread_pool_size );
                    }
                }
            }
//            ();
            this->wait();
        }

    private:

        std::vector<std::unique_ptr<Worker>> m_thread_pool; ///< Collection of workers
        std::atomic<sti> m_actively_working; ///< Counter of actively working workers
        ttl::Flare m_threads_done; ///< Notified when all threads have finished.
    };

} // Namespace ttl

#endif // BATCHWORKER_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class BatchWorker
///
/// Usage of fir:
/// \code
/// std::vector<int> v;
/// v.resize(200);
///
/// ttl::BatchWorker w(8);
/// w.fir
/// (
///     0, 90, 5, [&v](int i)
///     {
///         v[i] = i;
///     }
/// );
/// \endcode
///
/// In the above example, we have filled every 5th element,
/// starting at 0 and ending at less than 90, with its
/// locational representation.
/// You can accept a copy of the iterator, or a const-
/// reference as the lambda argument.
///
/// Here is another, easier interface:
///
/// \code
/// std::vector<int> v;
/// v.resize(200);
/// // ...
/// ttl::BatchWorker w(8);
/// std::mutex m;
/// w.fir
/// (
///     v, [&m](decltype(v)::iterator it)
///     {
///         std::lock_guard<std::mutex> l(m);
///         std::cout << *it << std::endl;
///     }
/// );
/// \endcode
///
/// The above code will iterate from the container's begin
/// to end. It is therefore possible to use this algorithm
/// with most container types, even sets, maps, and lists.
/// So long the container contains at least an input-
/// iterator class and a begin() and end() method.
///
/// The 4 fir functions are as follows:
/// 1. container, function
/// 2. begin, end, function
/// 3. container, step, function
/// 4. begin, end, step, function
///
////////////////////////////////////////////////////////////
