#ifndef SYNCHEDWRITER_HPP_INCLUDED
#define SYNCHEDWRITER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
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
        std::size_t getReaderCount() const
        {
            return m_synch.readers.load();
        }

    private:

        SynchedData &m_synch; ///< The data used to synchronize
        T &m_data; ///< The raw data object

    };

} // Namespace ttl

#endif // SYNCHEDWRITER_HPP_INCLUDED
