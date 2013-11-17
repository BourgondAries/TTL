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
#include "Bool/Bool.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Bool::Bool()
    {}

    ////////////////////////////////////////////////////////////
    Bool::Bool(const bool arg)
    :
        m_b(arg)
    {}

    ////////////////////////////////////////////////////////////
    void Bool::reset(bool state)
    {
        m_b = state;
    }

    ////////////////////////////////////////////////////////////
    bool Bool::fetch_and_enable()
    {
        if (m_b)
        {
            return m_b;
        }
        else
        {
            m_b = true;
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Bool::fetch_and_disable()
    {
        if (m_b)
        {
            m_b = false;
            return true;
        }
        else
        {
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Bool::fetch_and_flip()
    {
        if (m_b)
        {
            m_b = false;
            return true;
        }
        else
        {
            m_b = true;
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Bool::fetch_and_set(bool state)
    {
        if (m_b)
        {
            m_b = state;
            return true;
        }
        else
        {
            m_b = state;
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    Bool::operator bool()
    {
        return m_b;
    }

} // Namespace ttl
