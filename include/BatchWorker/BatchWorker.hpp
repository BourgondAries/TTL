/*
Copyright 2013-2014 Kevin Robert Stravers

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
#include <TTL/Ttldef/Ttldef.hpp>
#include <TTL/Sleep/Sleep.hpp>
#include <atomic>
#include <vector>
#include <cassert>
#include <iostream>
#include <TTL/Flare/Flare.hpp>
#include <TTL/Worker/Worker.hpp>
#include <TTL/Bool/Bool.hpp>


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
        BatchWorker(const Sti_t worker_count);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Set the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        void setWorkerCount(const Sti_t workers);

        ////////////////////////////////////////////////////////////
        /// \brief Get the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        Sti_t getWorkerCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Parallel for with thread IDs.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ITERATOR, typename FUNCTION>
        void fir(ITERATOR begin, ITERATOR end, FUNCTION fun, bool wait_for_all = true, bool main_contribute = true, const Sti_t advance = 1)
        {
            static_assert(is_iterator<ITERATOR>::value, "Arguments begin and end are not valid iterators.");
            if (begin == end)
            {
                return;
            }

            const Sti_t thread_pool_size(m_thread_pool.size());
            if (m_has_waited.fetchAndSet(wait_for_all) == false)
            {
                this->wait();
            }
            m_actively_working += thread_pool_size;
            if (wait_for_all == false) // Then main can exit the function, descoping the references
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, thread_pool_size, begin, end, fun, advance, main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                std::advance(start, advanceperi);
                                fun( *start, i );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    std::advance(start, advancepertps);
                                    fun( *start, i );
                                }
                            }
                        },
                        i
                    );
                }
            }
            else // We can rest assured; and take references.
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, &thread_pool_size, &begin, &end, &fun, &advance, &main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                std::advance(start, advanceperi);
                                fun( *start, i );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    std::advance(start, advancepertps);
                                    fun( *start, i );
                                }
                            }
                        },
                        i
                    );
                }
            }
            if (main_contribute)
            {
                Sti_t advanceperi = (thread_pool_size) * advance;

                ITERATOR start(begin);
                if (std::distance(start, end) > advanceperi)
                {
                    std::advance(start, advanceperi);
                    fun( *start, thread_pool_size );

                    Sti_t advancepertps = (thread_pool_size + 1) * advance;
                    while (std::distance(start, end) > advancepertps)
                    {
                        std::advance(start, advancepertps);
                        fun( *start, thread_pool_size );
                    }
                }
            }
            if (wait_for_all)
            {
                this->wait();
            }

        }


        ////////////////////////////////////////////////////////////
        /// \brief Parallel for.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ITERATOR, typename FUNCTION>
        void fer(ITERATOR begin, ITERATOR end, FUNCTION fun, bool wait_for_all = true, bool main_contribute = true, const Sti_t advance = 1)
        {
            static_assert(is_iterator<ITERATOR>::value, "Arguments begin and end are not valid iterators.");
            if (begin == end)
            {
                return;
            }

            const Sti_t thread_pool_size(m_thread_pool.size());
            if (m_has_waited.fetchAndSet(wait_for_all) == false)
            {
                this->wait();
            }
            m_actively_working += thread_pool_size;
            if (wait_for_all == false) // Then main can exit the function, descoping the references
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, thread_pool_size, begin, end, fun, advance, main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                std::advance(start, advanceperi);
                                fun( *start );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    std::advance(start, advancepertps);
                                    fun( *start );
                                }
                            }
                        },
                        i
                    );
                }
            }
            else // We can rest assured; and take references.
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, &thread_pool_size, &begin, &end, &fun, &advance, &main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                std::advance(start, advanceperi);
                                fun( *start );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    std::advance(start, advancepertps);
                                    fun( *start );
                                }
                            }
                        },
                        i
                    );
                }
            }
            if (main_contribute)
            {
                Sti_t advanceperi = (thread_pool_size) * advance;

                ITERATOR start(begin);
                if (std::distance(start, end) > advanceperi)
                {
                    std::advance(start, advanceperi);
                    fun( *start );

                    Sti_t advancepertps = (thread_pool_size + 1) * advance;
                    while (std::distance(start, end) > advancepertps)
                    {
                        std::advance(start, advancepertps);
                        fun( *start );
                    }
                }
            }
            if (wait_for_all)
            {
                this->wait();
            }
        }

        ////////////////////////////////////////////////////////////
        /// \brief Parallel for.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ITERATOR, typename FUNCTION>
        void fer2(ITERATOR begin, ITERATOR end, FUNCTION fun, bool wait_for_all = true, bool main_contribute = true, const Sti_t advance = 1)
        {
            static_assert(is_iterator<ITERATOR>::value, "Arguments begin and end are not valid iterators.");
            if (begin == end)
            {
                return;
            }

            const Sti_t thread_pool_size(m_thread_pool.size());
            if (m_has_waited.fetchAndSet(wait_for_all) == false)
            {
                this->wait();
            }
            m_actively_working += thread_pool_size;
            if (wait_for_all == false) // Then main can exit the function, descoping the references
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, thread_pool_size, begin, end, fun, advance, main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                start += advanceperi;
                                fun( *start );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    start += advancepertps;
                                    fun( *start );
                                }
                            }
                        },
                        i
                    );
                }
            }
            else // We can rest assured; and take references.
            {
                for (Sti_t i(0); i < thread_pool_size; ++i)
                {
                    this->issueWorkManualIncrement
                    (
                        [i, &thread_pool_size, &begin, &end, &fun, &advance, &main_contribute]() -> void
                        {
                            Sti_t advanceperi = i * advance;

                            ITERATOR start(begin);
                            if (std::distance(start, end) > advanceperi)
                            {
                                start += advanceperi;
                                fun( *start );

                                Sti_t advancepertps = (thread_pool_size + (main_contribute ? 1 : 0)) * advance;
                                while (std::distance(start, end) > advancepertps)
                                {
                                    start += advancepertps;
                                    fun( *start );
                                }
                            }
                        },
                        i
                    );
                }
            }
            if (main_contribute)
            {
                Sti_t advanceperi = (thread_pool_size) * advance;

                ITERATOR start(begin);
                if (std::distance(start, end) > advanceperi)
                {
                    start += advanceperi;
                    fun( *start );

                    Sti_t advancepertps = (thread_pool_size + 1) * advance;
                    while (std::distance(start, end) > advancepertps)
                    {
                        start += advancepertps;
                        fun( *start );
                    }
                }
            }
            if (wait_for_all)
            {
                this->wait();
            }
        }

    private:

        ////////////////////////////////////////////////////////////
        template<typename T, typename = void>
        struct is_iterator
        {static constexpr bool value = false;};

        ////////////////////////////////////////////////////////////
        template<typename T>
        struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
        {static constexpr bool value = true;};

        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWorkManualIncrement(const T t, const Sti_t thread)
        {
            m_thread_pool[thread]->issueWork
            (
                [t, this]()
                {
                    t();
                    if (m_actively_working.fetch_sub(1) == 1)
                    {
                        m_threads_done.notify();
                    }
                }
            );
        }

        ////////////////////////////////////////////////////////////
        void wait();

        ////////////////////////////////////////////////////////////
        std::vector<std::unique_ptr<Worker>> m_thread_pool; ///< Collection of workers
        std::atomic<Sti_t> m_actively_working; ///< Counter of actively working workers
        ttl::Flare m_threads_done; ///< Notified when all threads have finished.
        Bool m_has_waited;

    };

} // Namespace ttl


#endif // BATCHWORKER_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class BatchWorker
///
/// Usage of fer:
/// \code
/// std::vector<int> v;
/// v.resize(200);
///
/// ttl::BatchWorker w(8);
/// w.fer
/// (
///     ttl::Sit(0), ttl::Sit(90), [&v](std::size_t i)
///     {
///         v[i] = i;
///     }, true, true, 5
/// );
/// \endcode
///
/// In the above example, we have filled every 5th element,
/// starting at 0 and ending at less than 90, with its
/// locational representation.
/// You can accept a copy of the dereferenced iterator, or a
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
/// w.fer
/// (
///     v.begin(), v.end(), [&m](int n)
///     {
///         std::lock_guard<std::mutex> l(m);
///         std::cout << n << std::endl;
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
/// If for some reason you want the thread's ID as a
/// second argument, you can use "fir":
///
/// \code
/// w.fir
/// (
///     ttl::Sit(0), ttl::Sit(30),
///     [](std::size_t n, std::size_t thread_number)
///     {
///         std::cout << thread_number << std::endl;
///     }
/// );
/// \endcode
///
/// There are also 3 optional arguments that can come after
/// the lambda/function. These are:
/// "wait_for_all"
/// "main_contribute"
/// and
/// "advance"
///
/// wait_for_all instructs whether to wait for all threads
/// to finish working after they have been assigned work.
/// Waiting happens at the end of the function.
///
/// If you do not wait the first time, and then wait;
/// behaviour will be well-defined, and threads
/// will always be able to finish their work before the next
/// call to fir/fer, regardless of waiting or not at the end.
///
/// main_contribute == true causes the calling thread to
/// also contribute an equal amount of work as the workers.
/// Turning this off may be wise when a large number of threads,
/// when the caller spends most of its time delegating jobs.
///
/// Combining the above two will result in only a for loop
/// which delegates and exits, which may be useful if the
/// caller needs to do other things quickly after delegating.
///
/// advance tells the function what multiples to run the function
/// for. if advance is 2, then 0, 2, 4, 6,... will be run.
/// If advance = 3, then 0, 3, 6, 9,... will be run.
/// 0 is an included multiple.
///
////////////////////////////////////////////////////////////
