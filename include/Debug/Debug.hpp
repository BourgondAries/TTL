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


#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

// Headers
#include <exception>
#include <string>


#if !defined(TTL_DEBUG) && defined(NDEBUG)
    #define TTL_DEBUG_THROW(x)
    #define TTL_DEBUG_IF(x)
    #define TTL_DEBUG_ELSE
    #define TTL_DEBUG_SCOPE(x)
#else
    #define TTL_DEBUG_THROW(x)                          \
        throw std::runtime_error                        \
        (                                               \
            std::string("START OF EXCEPTION MESSAGE")   \
            + "\nMessage: "                             \
            + x                                         \
            + "\nFile: "                                \
            + __FILE__                                  \
            + "\nFunction: "                            \
            + __func__                                  \
            + "\nLine: "                                \
            + std::to_string(__LINE__)                  \
            + std::string("\nEND OF EXCEPTION MESSAGE")    \
        )
    #define TTL_DEBUG_IF(x) if(x)
    #define TTL_DEBUG_ELSE else
    #define TTL_DEBUG_SCOPE(x) x
#endif // NDEBUG


#endif // DEBUG_HPP_INCLUDED
