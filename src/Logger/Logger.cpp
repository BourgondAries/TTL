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


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Logger/Logger.hpp"


namespace ttl
{
    // TRUE PART ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    Logger<true>::Logger(const char *filename, bool to_clog, std::ios::openmode std_ios_flags)
    :
    output(filename, std_ios_flags),
    clog_it(to_clog)
    {}

    ////////////////////////////////////////////////////////////
    Logger<true>::Logger(std::string &filename, bool to_clog, std::ios::openmode std_ios_flags)
    :
    output(filename.data(), std_ios_flags),
    clog_it(to_clog)
    {}

    ////////////////////////////////////////////////////////////
    Logger<true>::~Logger()
    {
        output.close();
    }


    // FALSE PART ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    Logger<false>::Logger(const char *filename, bool to_clog, std::ios::openmode std_ios_flags)
    {}

    ////////////////////////////////////////////////////////////
    Logger<false>::Logger(std::string &filename, bool to_clog, std::ios::openmode std_ios_flags)
    {}

    ////////////////////////////////////////////////////////////
    Logger<false>::~Logger()
    {}

} // Namespace ttl
