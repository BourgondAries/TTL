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


#ifndef TIMESTAMP_HPP_INCLUDED
#define TIMESTAMP_HPP_INCLUDED

// Headers
#include <string>
#include <chrono>
#include <ctime>

namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief returns a time stamp string to the nanosecond
    ///
    ////////////////////////////////////////////////////////////
    std::string getTimeStamp();

    ////////////////////////////////////////////////////////////
    /// \brief Dummy variable used in Logger for overloads.
    ///
    ////////////////////////////////////////////////////////////
    struct m_Timestamp {} static constexpr const Timestamp = m_Timestamp();

} // Namespace ttl

#endif // TIMESTAMP_HPP_INCLUDED
