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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Ips/Ips.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Ips::Ips()
    :
    min_time(0),
    delay(0),
    t1(hre::now()),
    t2(t1)
    {}

    ////////////////////////////////////////////////////////////
    Ips::Ips(const float ips)
    :
    min_time(static_cast<long int>(1E6f / ips)),
    delay(0),
    t1(hre::now()),
    t2(t1)
    {}

    ////////////////////////////////////////////////////////////
    Ips::~Ips()
    {}

    ////////////////////////////////////////////////////////////
    void Ips::limit()
    {
        t2 = hre::now();
        delay = std::chrono::duration_cast<us>(t2 - t1);

        // We now know the delay, let's check it against our sleep time:
        if (delay < min_time)
            std::this_thread::sleep_for(min_time - delay);

        // Reset the starting timer.
        t1 = hre::now();
    }

    ////////////////////////////////////////////////////////////
    float Ips::getIps() const
    {
        return min_time.count() / 1E6f;
    }

    ////////////////////////////////////////////////////////////
    void Ips::setIps(const float ips)
    {
        min_time = us(static_cast<long int>(1E6f / ips));
    }

    ////////////////////////////////////////////////////////////
    Ips::us Ips::getMinIterationTime() const
    {
        return min_time;
    }

    ////////////////////////////////////////////////////////////
    void Ips::setMinIterationTime(const Ips::us &microseconds)
    {
        min_time = microseconds;
    }

    ////////////////////////////////////////////////////////////
    Ips::us Ips::getDelay() const
    {
        return delay;
    }

    ////////////////////////////////////////////////////////////
    std::ostream &operator<<(std::ostream &lhs, Ips &rhs)
    {
        typedef std::chrono::microseconds us;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds s;
        typedef std::chrono::minutes m;
        typedef std::chrono::hours h;

        lhs
            << "Ips:" << std::endl << "\tIT = "; // Iteration time

        if (rhs.delay > h(1))
            lhs << rhs.delay.count() / (1E6f * 3600.f) << " h" << std::endl;
        else if (rhs.delay > m(1))
            lhs << rhs.delay.count() / (1E6f * 60.f) << " min" << std::endl;
        else if (rhs.delay > s(1))
            lhs << rhs.delay.count() / 1E6f << " s" << std::endl;
        else if (rhs.delay > ms(1))
            lhs << rhs.delay.count() / 1E3f << " ms" << std::endl;
        else
            lhs << rhs.delay.count() << " µs" << std::endl;

        lhs
            << "\tRTPI = "; // Requested Time Per Iteration

        if (rhs.min_time > h(1))
            lhs << rhs.min_time.count() / (1E6f * 3600.f) << " h" << std::endl;
        else if (rhs.min_time > m(1))
            lhs << rhs.min_time.count() / (1E6f * 60.f) << " min" << std::endl;
        else if (rhs.min_time > s(1))
            lhs << rhs.min_time.count() / 1E6f << " s" << std::endl;
        else if (rhs.min_time > ms(1))
            lhs << rhs.min_time.count() / 1E3f << " ms" << std::endl;
        else
            lhs << rhs.min_time.count() << " µs" << std::endl;

        return lhs;
    }

} // Namespace ttl
