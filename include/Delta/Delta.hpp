#ifndef DELTA_HPP_INCLUDED
#define DELTA_HPP_INCLUDED

#include <iostream>

namespace ttl
{

    template <typename T>
    class Delta
    {
    public:

        Delta(const T lower, const T upper)
        :
            m_lower(lower),
            m_upper(upper)
            {}

        Delta(const T lower, const T upper, const T data)
        :
            m_data(data),
            m_lower(lower),
            m_upper(upper)
            {
                checkBounds();
            }

        Delta<T> &operator=(const Delta<T> &delta)
        {
            m_data = delta.m_data;
            m_lower = delta.m_lower;
            m_upper = delta.m_upper;
            m_rev = delta.m_rev;
        }

        Delta<T> &operator=(const T val)
        {
            m_data = val;
            checkBounds();
        }

        explicit operator T()
        {
            return m_data;
        }

        void setRange(const T lower, const T upper)
        {
            m_lower = lower;
            m_upper = upper;
        }

        Delta<T> &operator+=(T val)
        {
            if (val > m_upper)
            {
                val = m_upper - (val - m_upper);
            }
            else if (val < m_lower)
            {
                val = m_lower - (val - m_lower);
            }

            if (!m_rev)
            {
                m_data += val;
            }
            else
            {
                m_data -= val;
            }
            checkBounds();
            return *this;
        }


        Delta<T> &operator-=(T val)
        {
            if (val > m_upper)
            {
                val = m_upper - (val - m_upper);
            }
            else if (val < m_lower)
            {
                val = m_lower - (val - m_lower);
            }

            if (!m_rev)
            {
                m_data -= val;
            }
            else
            {
                m_data += val;
            }
            checkBounds();
            return *this;
        }

    private:

        void checkBounds()
        {
            if (m_data > m_upper)
            {
                m_data = m_upper - (m_data - m_upper);
                m_rev = !m_rev;
            }
            else if (m_data < m_lower)
            {
                m_data = m_lower - (m_data - m_lower);
                m_rev = !m_rev;
            }
        }

        T       m_data,
                m_lower,
                m_upper;
        bool    m_rev = false;

    };

} // Namespace ttl


#endif // DELTA_HPP_INCLUDED
