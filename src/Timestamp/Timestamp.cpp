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
#include <Timestamp/Timestamp.hpp>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    std::string getTimeStamp()
    {
        typedef std::chrono::system_clock chrsc;
        typedef std::chrono::time_point<chrsc> chrtp;

        chrtp now = std::chrono::system_clock::now();
        chrtp epoch;

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - epoch).count();

        std::time_t time = std::chrono::system_clock::to_time_t(now);

        duration %= 1000000000;
        std::string str(std::ctime(&time));

        std::string::iterator it = --str.end();
        for (; it != str.begin(); --it)
        {
            if (*it == ' ')
            {
                break;
            }
        }

        // Make sure duration has enough prefix 0:
        std::string duration_str = std::move(std::to_string(duration));
        while (duration_str.size() < 9)
            duration_str.insert(duration_str.begin(), '0');

        std::string dur = "." + duration_str;
        str.insert(it, dur.begin(), dur.end());
        str.erase(--str.end());

        return str;
    }

} // Namespace ttl
