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
