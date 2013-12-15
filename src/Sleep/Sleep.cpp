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


// Headers
#include <thread>
#include <chrono>
#include "Ttldef/Ttldef.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    void sleep(const sti sec)
    {
        std::this_thread::sleep_for(std::chrono::seconds(sec));
    }

    ////////////////////////////////////////////////////////////
    void msleep(const sti msec)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

    ////////////////////////////////////////////////////////////
    void usleep(const sti usec)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(usec));
    }

} // Namespace ttl
