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


#ifndef MIXIN_HPP_INCLUDED
#define MIXIN_HPP_INCLUDED

// Headers
#include "../include/Ttldef/Ttldef.hpp"
#include <tuple>
#include <type_traits>
#include <utility>


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief A class for mixing classes with eachother
    ///
    ////////////////////////////////////////////////////////////
    template <typename ...CLASSES>
    class Mixin : public CLASSES...
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// Using template arguments, constructs the classes
        ///
        ////////////////////////////////////////////////////////////
        explicit Mixin()
        :
        CLASSES()...
        {}

    //    ////////////////////////////////////////////////////////////
    //    /// \brief Constructor
    //    ///
    //    /// Supposed to work with arguments,... unfinished.
    //    ///
    //    ////////////////////////////////////////////////////////////
    //    template <typename ...Args>
    //    Mixin(Args &&...args)
    //    :
    //    CLASSES(std::forward<Args>(args))...
    //    {}
    };

} // Namespace ttl

#endif // MIXIN_HPP_INCLUDED
