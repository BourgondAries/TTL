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


#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include "Synched/Synched.hpp"
#include "Valman/Valman.hpp"

namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief a convenience function to read from synched valmans
    ///
    /// \param arg The synched value manager to read from
    /// \param key The index to retrieve data from
    /// \return A string containing the data
    ///
    ////////////////////////////////////////////////////////////
    std::string read(const Synched<Valman> &arg, const std::string &key);

    ////////////////////////////////////////////////////////////
    /// \brief a convenience function to read from synched valmans
    ///
    /// This function also checks for the key's validity
    /// The check makes this function slower, but safer.
    ///
    /// \param arg The synched value manager to read from
    /// \param key The index to retrieve data from
    /// \return A string containing the data
    ///
    ////////////////////////////////////////////////////////////
    std::string sread(const Synched<Valman> &arg, const std::string &key);

    ////////////////////////////////////////////////////////////
    /// \brief a convenience function to read from synched valmans
    ///
    /// This function also checks for the key's validity
    /// If the check fails, this function throws.
    ///
    /// \param arg The synched value manager to read from
    /// \param key The index to retrieve data from
    /// \return A string containing the data
    ///
    ////////////////////////////////////////////////////////////
    std::string tread(const Synched<Valman> &arg, const std::string &key);

}

#endif // UTILITIES_HPP_INCLUDED
