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

#ifndef SYNCHED_HPP_INCLUDED
#define SYNCHED_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <mutex>
#include "SynchedWriter.hpp"
#include "SynchedReader.hpp"
#include "SynchedData.hpp"


namespace ttl
{

    template <typename T>
    class Synched
    {
    public:

        Synched() = default; // Nothing to do

        template <typename ...Args>
        Synched(Args ...args)
        :
            m_data(std::forward<Args>(args)...)
        {}

        Synched(T &in) // Assign reference using the copy ctor
        :
            m_data(in)
        {}

        Synched(T &&in) // Assign an r-value reference
        :
            m_data(in)
        {}

        Synched(const Synched &copy) = delete;
        Synched &operator=(const Synched &copy) = delete;

        ~Synched() = default; // Nothing to do...

        SynchedWriter<T> getWriteAccess() {return SynchedWriter<T>(m_synch, m_data);}
        const SynchedReader<T> getReadAccess() {return SynchedReader<T>(m_synch, m_data);}

        std::size_t getReaderCount() const
        {
            return m_synch.readers.load();
        }

    private:

        SynchedData m_synch;
        T m_data; // Raw data
    };

} // Namespace ttl

#endif // SYNCHED_HPP_INCLUDED
