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

    ////////////////////////////////////////////////////////////
    /// \brief Encapsulator that makes objects thread safe
    ///
    /// Allows multiple reads, but only single writes.
    /// This implementation is ALWAYS thread-safe, unless
    /// multiple readers edit mutable variables simultaneously.
    /// This depends on the class at hand. If your class has
    /// mutable fields, then make sure to make all const
    /// functions take note of that (and protect it).
    /// Another option is to make the mutable a Synched<>
    /// object.
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    class Synched
    {
    public:
        
        ////////////////////////////////////////////////////////////
        /// \brief Default ctor
        ///
        /// Will invoke the data's standard ctor.
        /// Can be garbage data.
        ///
        ////////////////////////////////////////////////////////////
        Synched() = default;

        ////////////////////////////////////////////////////////////
        /// \brief Argument ctor
        ///
        /// \param args Forwards the arguments to the constructor
        /// of the data specified in the template.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ...Args>
        Synched(Args ...args)
        :
            m_data(std::forward<Args>(args)...)
        {}

        ////////////////////////////////////////////////////////////
        /// \brief Copy ctor
        ///
        /// Copies the data an external source
        ///
        ////////////////////////////////////////////////////////////
        Synched(const T &in) // Assign reference using the copy ctor
        :
            m_data(std::forward<T>(in))
        {}

        ////////////////////////////////////////////////////////////
        /// \brief Move ctor
        ///
        /// Copies the data from an r-value reference.
        ///
        ////////////////////////////////////////////////////////////
        Synched(T &&in) // Assign an r-value reference
        :
            m_data(in)
        {
            in = T();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Copy ctor
        ///
        /// Copies the data contained in another Synched object
        ///
        ////////////////////////////////////////////////////////////
        Synched(const Synched &copy)
        :
            m_data(*(copy.getReadAccess()))
        {}

        ////////////////////////////////////////////////////////////
        /// \brief Move ctor
        ///
        /// Moves the data contained in another Synched object
        ///
        ////////////////////////////////////////////////////////////
        Synched(Synched &&copy)
        :
            m_data(*(copy.getReadAccess()))
        {
            *(copy.getWriteAccess()) = T();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Copy assignment
        ///
        /// Copies the data contained in another Synched object
        ///
        /// \return A reference to itself
        ///
        ////////////////////////////////////////////////////////////
        Synched &operator=(const Synched &copy)
        {
            *(this->getWriteAccess()) = *(copy.getReadAccess());
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Move assignment
        ///
        /// Copies the data contained in another Synched object
        ///
        /// \return A reference to itself
        ///
        ////////////////////////////////////////////////////////////
        Synched &operator=(Synched &&copy)
        {
            SynchedWriter<T> wexed = copy.getWriteAccess();
            *(this->getWriteAccess()) = *(wexed);
            *(wexed) = T();
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Everything is RAII, no need to do anything
        ///
        ////////////////////////////////////////////////////////////
        ~Synched() = default;

        ////////////////////////////////////////////////////////////
        /// \brief returns a SynchedWriter
        ///
        /// Waits for all SynchedReaders to be destroyed.
        /// The SynchedWriter will block all other writers and readers
        /// until its destructor has been called.
        ///
        /// \return a SynchedWriter object
        ///
        ////////////////////////////////////////////////////////////
        SynchedWriter<T> getWriteAccess()
        {
            return SynchedWriter<T>(m_synch, m_data);
        }

        ////////////////////////////////////////////////////////////
        /// \brief returns a SynchedReader
        ///
        /// The SynchedReader will block all SynchedWriters
        /// until all SynchedReaders have been destroyed.
        ///
        /// \return a SynchedReader object
        ///
        ////////////////////////////////////////////////////////////
        const SynchedReader<T> getReadAccess() const
        {
            return SynchedReader<T>(m_synch, m_data);
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

        mutable SynchedData m_synch; ///< The data used to synchronize
        T m_data; ///< The raw data object
    };

} // Namespace ttl

#endif // SYNCHED_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Synched
/// \ingroup Thread Utilities
///
/// Using Synched will allow for multiple readers to read,
/// whilst only a single writer can write at a time.
/// The protective method is the same as the one
/// employed by std::weak_ptr. This method uses a
/// temporary object.
///
/// \code
/// ttl::Synched<int> xxx(32);
///
/// // From thread 1:
/// auto x = xxx.getReadAccess(); // Reader object, multiple can exist simultaneously.
/// std::cout << *x << std::endl;
///
/// // From thread 2:
/// auto x = xxx.getWriteAccess(); // Writer object, no more than 1 can exist simultaneously.
/// (*x)++;
///
/// \endcode
////////////////////////////////////////////////////////////
