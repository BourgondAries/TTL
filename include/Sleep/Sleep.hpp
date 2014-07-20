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


#ifndef SLEEP_HPP_INCLUDED
#define SLEEP_HPP_INCLUDED

// Headers
#include <TTL/Ttldef/Ttldef.hpp>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief sleep for specified seconds
    ////////////////////////////////////////////////////////////
    extern void sleep(Sti_t sec);

    ////////////////////////////////////////////////////////////
    /// \brief sleep for specified milliseconds
    ////////////////////////////////////////////////////////////
    extern void msleep(Sti_t msec);

    ////////////////////////////////////////////////////////////
    /// \brief sleep for specified microseconds
    ////////////////////////////////////////////////////////////
    extern void usleep(Sti_t usec);

} // Namespace ttl

#endif // SLEEP_HPP_INCLUDED
