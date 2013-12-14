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


#ifndef RTC_HPP_INCLUDED
#define RTC_HPP_INCLUDED

// Headers
#include "TTL/Debug/Debug.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Run-Time Const
    ///
    /// Allows an object to be manipulated until it
    /// is turned const.
    ////////////////////////////////////////////////////////////
    template <typename CLASS>
    class Rtc
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Allows one to write to the object
        /// \return A non-const reference to the object
        ////////////////////////////////////////////////////////////
        CLASS &write()
        {
            TTL_DEBUG_IF(m_is_const)
                TTL_DEBUG_THROW("Rtc: The instance is const and can thus not be written to");
            return m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Allows one to read from the object
        /// \return A const reference to the object
        ////////////////////////////////////////////////////////////
        const CLASS &read() const
        {
            return m_data;
        }

        ////////////////////////////////////////////////////////////
        /// \brief "Hardens" the object by turning it const
        ///
        /// After this operation is complete, the object will
        /// throw an error upon trying to write, or trying to
        /// turning it const again.
        ////////////////////////////////////////////////////////////
        void turnConst()
        {
            TTL_DEBUG_IF(m_is_const)
                TTL_DEBUG_THROW("Rtc: Can not turn the instance const; already const");
            m_is_const = true;
        }

    private:

        CLASS m_data; ///< The actual data
        bool m_is_const = false; ///< The flag that defines constness

    };

} // Namespace ttl

#endif // RTC_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Rtc
///
/// Sample:
/// \code
/// int main(int argc, char ///argv[])
/// {
///     using ttl::Rtc;
///     using ttl::Argument;
///     using ttl::Singleton;
///     auto &a = Singleton<Rtc<Argument>>::get();
///     a.write().setInert('h');
///     a.write().pass(argc, argv);
///     a.turnConst();
///
///     beginMagic();
/// }
/// \endcode
///
/// What does this allow? A global argument list accessible from anywhere.
/// The arguments can not be modified, but the options can be set in the start.
////////////////////////////////////////////////////////////
