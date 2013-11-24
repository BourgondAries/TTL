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

    /**
    * \brief Run-Time Const
    *
    * Allows an object to be manipulated until it
    * is turned const.
    */
    template <typename CLASS>
    class Rtc
    {
    public:

        /**
        * \brief Allows one to write to the object
        * \return A non-const reference to the object
        */
        CLASS &write()
        {
            TTL_DEBUG_IF(m_is_const)
                TTL_DEBUG_THROW("The instance is const and can not be written to");
            return m_data;
        }

        /**
        * \brief Allows one to read from the object
        * \return A const reference to the object
        */
        const CLASS &read() const
        {
            return m_data;
        }

        /**
        * \brief "Hardens" the object by turning it const
        *
        * After this operation is complete, the object will
        * throw an error upon trying to write, or trying to
        * turning it const again.
        */
        void turnConst()
        {
            TTL_DEBUG_IF(m_is_const)
                TTL_DEBUG_THROW("Can not turn the instance const; already const");
            m_is_const = true;
        }

    private:

        CLASS m_data; ///< The actual data
        bool m_is_const = false; ///< The flag that defines constness

    };

} // Namespace ttl

#endif // RTC_HPP_INCLUDED
