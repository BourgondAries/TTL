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


#ifndef SYNCHEDREADER_HPP_INCLUDED
#define SYNCHEDREADER_HPP_INCLUDED

// Headers
#include <mutex>
#include "SynchedData.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief The reader object
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    class SynchedReader
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// Takes a reference to the data and Synchronization
        /// primitives of Synched.
        ///
        /// \param synch The synchronization struct
        /// \param data The data to reference
        ///
        ////////////////////////////////////////////////////////////
        SynchedReader(SynchedData &synch, const T &data)
        :
            m_synch(synch),
            m_data(data)
        {
            m_synch.entry_mutex.lock();
            m_synch.entry_mutex.unlock();
            if (m_synch.readers.fetch_add(1) == 0)
                m_synch.writer_activation.wait();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Decrements the reference count of readers atomically
        ///
        ////////////////////////////////////////////////////////////
        ~SynchedReader()
        {
            if (m_synch.readers.fetch_sub(1) == 1)
            {
                m_synch.writer_activation.notify();
            }
        }

        ////////////////////////////////////////////////////////////
        /// \brief Data extraction
        ///
        /// \return A const pointer to the data.
        ///
        ////////////////////////////////////////////////////////////
        const T *operator->() const
        {
            return &m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Data extraction
        ///
        /// \return A const reference to the data.
        ///
        ////////////////////////////////////////////////////////////
        const T &operator*() const
        {
            return m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief The count of readers
        ///
        /// \return the amount of readers currently reading the data.
        ///
        ////////////////////////////////////////////////////////////
        sti getReaderCount() const
        {
            return m_synch.readers.load();
        }

    private:

        SynchedData &m_synch; ///< The data used to synchronize
        const T &m_data; ///< The raw data object

    };

} // Namespace ttl


#endif // SYNCHEDREADER_HPP_INCLUDED
