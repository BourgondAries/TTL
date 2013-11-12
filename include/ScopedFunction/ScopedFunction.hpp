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


#ifndef SCOPEDFUNCTION_HPP_INCLUDED
#define SCOPEDFUNCTION_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Executes the function upon scope exit
    ///
    /// This RAII wrapper of functions executes a function on
    /// scope exit. This can be used ensure exception safety.
    ///
    ////////////////////////////////////////////////////////////
    class ScopedFunction
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Empty Constructor
        ///
        /// Will construct an empty function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction();

        ////////////////////////////////////////////////////////////
        /// \brief Copy Constructor
        ///
        /// Will copy the function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction(const ScopedFunction &scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Move Constructor
        ///
        /// Will copy the function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction(ScopedFunction &&scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Constructor that takes a function
        ///
        /// Can take bind expressions as well. Function is executed
        /// at scope exit unless reset.
        ///
        /// \param args Arguments to the bind expression.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ...Args>
        ScopedFunction(Args &&...args)
        {
            auto f = std::bind(std::forward<Args>(args)...);
            fn = [=]()
            {
                f();
            };
        }

        ////////////////////////////////////////////////////////////
        /// \brief Copy Assignment
        ///
        /// \param scopedfunction The ScopedFunction to copy
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction &operator=(const ScopedFunction &scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Move Assignment
        ///
        /// \param scopedfunction The ScopedFunction to move from
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction &operator=(ScopedFunction &&scopedfunction);

        void reset();

        template <typename ...Args>
        void reset(Args &&...args)
        {
            auto f = std::bind(std::forward<Args>(args)...);
            fn = [=]()
            {
                f();
            };
        }

        ~ScopedFunction();

    private:

        std::function<void()> fn;

    };

} // Namespace ttl


#endif // SCOPEDFUNCTION_HPP_INCLUDED
