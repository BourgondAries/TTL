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


#ifndef FILE2STR_HPP_INCLUDED
#define FILE2STR_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief loads the contents of an entire file into a string
    ///
    ////////////////////////////////////////////////////////////
    extern std::string file2str(const std::string &filename);

} // Namespace ttl

#endif // FILE2STR_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// Sample usage:
///
/// \code
/// std::string file = file2str("my_file.txt");
/// \endcode
///
////////////////////////////////////////////////////////////
