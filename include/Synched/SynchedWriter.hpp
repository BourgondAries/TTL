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


#ifndef SYNCHEDWRITER_HPP_INCLUDED
#define SYNCHEDWRITER_HPP_INCLUDED

// Headers
#include <mutex>
#include "SynchedData.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief The writer object
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    class SynchedWriter
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// Blocks new readers from being created,
        /// waits for all readers to finish.
        ///
        /// \param synch The synchronization struct
        /// \param data The data to reference
        ///
        ////////////////////////////////////////////////////////////
        SynchedWriter(SynchedData &synch, T &data)
        :
            m_synch(synch),
            m_data(data)
        {
            m_synch.entry_mutex.lock();
            m_synch.writer_activation.wait();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Allows readers to read again, or another writer to write.
        ///
        ////////////////////////////////////////////////////////////
        ~SynchedWriter()
        {
            m_synch.entry_mutex.unlock();
            m_synch.writer_activation.notify();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Data extraction
        ///
        /// \return A pointer to the data.
        ///
        ////////////////////////////////////////////////////////////
        T *operator->() const
        {
            return &m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Data extraction
        ///
        /// \return A reference to the data.
        ///
        ////////////////////////////////////////////////////////////
        T &operator*() const
        {
            return m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief The count of readers
        ///
        /// \return the amount of readers currently reading the data.
        ///
        ////////////////////////////////////////////////////////////
        Sti_t getReaderCount() const
        {
            return m_synch.readers.load();
        }

    private:

        SynchedData &m_synch; ///< The data used to synchronize
        T &m_data; ///< The raw data object

    };

} // Namespace ttl

#endif // SYNCHEDWRITER_HPP_INCLUDED
