////////////////////////////////////////////////////////////
//
// TTL - Tea Tank Library
// Copyright (C) 2013 Kevin R. Stravers (macocio@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef MIXIN_HPP_INCLUDED
#define MIXIN_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
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
