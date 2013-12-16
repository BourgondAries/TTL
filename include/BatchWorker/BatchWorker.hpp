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
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWork(T t, const sti thread)
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

        // Type-dependent iterator.
        template <typename T, bool IS_CLASS>
        class Citerator;

        // If we are supplied an int, char,... , we must create an iterator around it.
        template <typename TYPE>
        class Citerator<TYPE, false> : public std::iterator<std::input_iterator_tag, TYPE>
        {
        public:

            Citerator(TYPE iterator):m_raw_iterator(iterator){}
            Citerator(const Citerator &iterator):m_raw_iterator(iterator.m_raw_iterator){}
            Citerator &operator=(const Citerator &iterator){m_raw_iterator = iterator.m_raw_iterator; return *this;}

            bool operator==(const Citerator &citerator){return m_raw_iterator != citerator.m_raw_iterator;}
            bool operator!=(const Citerator &citerator){return m_raw_iterator != citerator.m_raw_iterator;}

            TYPE &operator*(){return m_raw_iterator;}
            TYPE &operator->(){return m_raw_iterator;}

            Citerator &operator++(){++m_raw_iterator; return *this;}
            Citerator &operator++(int){m_raw_iterator++; return *this;}

            TYPE &get(){return m_raw_iterator;}

        private:

            TYPE m_raw_iterator;

        };

        // If we are provided an iterator, we inherit from it.
        template <typename TYPE>
        class Citerator<TYPE, true> : public TYPE
        {
        public:
            using TYPE::TYPE;
            TYPE &get(){return *this;}
        };

        // The iterator that wraps around both integers and other iterators.
        template <typename TYPE, bool IS_CLASS = std::is_class<TYPE>::value>
        class Iterator : public Citerator<TYPE, IS_CLASS>
        {
        public:
            using Citerator<TYPE, IS_CLASS>::Citerator;
        };

    public:

        ////////////////////////////////////////////////////////////
        /// \brief A simple interface for performing parallel fors
        /// over any range
        ///
        /// Applies a specified function to [begin, end) elements.
        ///
        /// \param begin The start iterator.
        /// \param end The last iterator.
        /// \param advance The unsigned type that species how many
        /// steps each movement should take.
        /// \param function the function that accepts a
        /// dereferenced iterator as its parameter (which is a
        /// reference).
        ///
        ////////////////////////////////////////////////////////////
        template <typename ITERATOR, typename FUNCTION>
        void fir(ITERATOR begin, ITERATOR end, sti advance, FUNCTION fun)
        {
            sti thread_pool_size(m_thread_pool.size());
            for (sti i(0); i < thread_pool_size; ++i)
            {
                this->issueWork
                (
                    [i, &thread_pool_size, &begin, &end, &fun, &advance]() -> void
                    {
                        sti advanceperi = i * advance;

                        Iterator<ITERATOR> current(begin), death(end);
                        if (std::distance(current, death) > advanceperi)
                        {
                            std::advance(current, advanceperi);
                            fun(const_cast<const ITERATOR &>( current.get() ));

                            sti advancepertps = thread_pool_size * advance;
                            while (std::distance(current, death) > advancepertps)
                            {
                                std::advance(current, advancepertps);
                                fun(const_cast<const ITERATOR &>( current.get() ));
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
        void fir(ITERATOR begin, ITERATOR end, FUNCTION fun)
        {
            sti thread_pool_size(m_thread_pool.size());
            for (sti i(0); i < thread_pool_size; ++i)
            {
                this->issueWork
                (
                    [i, &thread_pool_size, &begin, &end, &fun]() -> void
                    {
                        Iterator<ITERATOR> current(begin), death(end);
                        if (std::distance(current, death) > i)
                        {
                            std::advance(current, i);
                            fun(current.get());
                            while (std::distance(current, death) > thread_pool_size)
                            {
                                std::advance(current, thread_pool_size);
                                fun(current.get());
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
        void fir(CONTAINER container, FUNCTION function)
        {
            fir(container.begin(), container.end(), function);
        }

        ////////////////////////////////////////////////////////////
        /// \brief A simple interface for performing parallel fors
        /// over standard-compliant iteratable containers.
        ///
        /// \param container The container to iterate over
        /// Requires CONTAINER::begin() and CONTAINER::end()
        /// which have an operator++(int) methods.
        /// \param advance the amount to advance each iteration.
        /// \param function the function that accepts a
        /// dereferenced iterator as its parameter (which is a
        /// reference).
        ///
        ////////////////////////////////////////////////////////////
        template <typename CONTAINER, typename FUNCTION>
        void fir(CONTAINER container, sti advance, FUNCTION function)
        {
            fir(container.begin(), container.end(), advance, function);
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
