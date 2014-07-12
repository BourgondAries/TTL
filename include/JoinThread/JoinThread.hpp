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


#ifndef JOINTHREAD_HPP_INCLUDED
#define JOINTHREAD_HPP_INCLUDED

// Headers
#include <thread>
#include <functional>


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief Scoped thread that joins at scope exit
    ///
    ////////////////////////////////////////////////////////////
    class JoinThread : public std::thread
    {
    public:

        using std::thread::thread;
        ////////////////////////////////////////////////////////////
        /// \brief Constructor delegator
        ///
        /// \see std::thread::thread(...)
        ///
        ////////////////////////////////////////////////////////////
//        JoinThread();

        ////////////////////////////////////////////////////////////
        /// \brief Constructor delegator
        ///
        /// \see std::thread::thread(...)
        ///
        ////////////////////////////////////////////////////////////
//        template <typename ...Args>
//        JoinThread(Args &&...args):std::thread(std::forward<Args>(args)...){}

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Joins the thread
        ///
        ////////////////////////////////////////////////////////////
        ~JoinThread();

    };

} // Namespace ttl

#endif // JOINTHREAD_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class JoinThread
/// \ingroup Utilities
///
/// This class works exactly like std::thread since it
/// inherits therefrom and delegates all constructor
/// arguments to std::thread::thread(...)
///
/// \code
/// int n = 0;
/// JoinThread thr
/// (
///     [](int &number)
///     {
///         number++;
///         std::this_thread::sleep_for(std::chrono::seconds(3));
///     },
///     std::ref(n)
/// );
/// \endcode
///
/// Don't worry about detaching the thread, a check is
/// performed during destruction to make sure the thread
/// is joinable.
///
/// \see std::thread
///
////////////////////////////////////////////////////////////
