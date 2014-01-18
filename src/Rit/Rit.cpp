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


// Headers
#include "Rit/Rit.hpp"
#include <limits>
#include <algorithm>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Rit::Rit()
    :
        m_dist1(1),
        m_dist2(1),
        m_requested_ratio(1 / static_cast<float>(1)),
        m_recorded_ratio(0.f),
        m_ia(1),
        m_ib(1)
    {}

    ////////////////////////////////////////////////////////////
    Rit::Rit(const Sti_t distribution_1, const Sti_t distribution_2)
    :
        m_dist1(distribution_1),
        m_dist2(distribution_2),
        m_requested_ratio(distribution_1 / static_cast<float>(distribution_2)),
        m_recorded_ratio(0.f),
        m_ia(1),
        m_ib(1)
    {}

    ////////////////////////////////////////////////////////////
    Rit::~Rit(){}

    ////////////////////////////////////////////////////////////
    void Rit::setDistribution(const Sti_t distribution_1, const Sti_t distribution_2)
    {
        m_dist1 = distribution_1;
        m_dist2 = distribution_2;
        m_requested_ratio = distribution_1 / static_cast<float>(distribution_2);
        m_recorded_ratio = 0.f;
        m_ia = 1;
        m_ib = 1;
    }

    ////////////////////////////////////////////////////////////
    Sti_t Rit::getFirstDistribution() const
    {
        return m_dist1;
    }

    ////////////////////////////////////////////////////////////
    Sti_t Rit::getSecondDistribution() const
    {
        return m_dist2;
    }

    ////////////////////////////////////////////////////////////
    float Rit::getRatio() const
    {
        return m_requested_ratio;
    }

    ////////////////////////////////////////////////////////////
    bool Rit::isFirstReady()
    {
        if (m_recorded_ratio <= m_requested_ratio)
        {
            ++m_ia;
            if (m_ia >= std::numeric_limits<decltype(m_ia)>::max() - 1)
            {
                m_ia %= m_dist1;
                if (m_ia == 0)
                    m_ia = 1;
                m_ib %= m_dist2;
                if (m_ib == 0)
                    m_ib = 1;
            }
            m_recorded_ratio = static_cast<float>(m_ia) / static_cast<float>(m_ib);
            return true;
        }
        return false;
    }

    ////////////////////////////////////////////////////////////
    bool Rit::isSecondReady()
    {
        if (m_recorded_ratio >= m_requested_ratio)
        {
            ++m_ib;
            if (m_ib >= std::numeric_limits<decltype(m_ib)>::max() - 1)
            {
                m_ia %= m_dist1;
                if (m_ia == 0)
                    m_ia = 1;
                m_ib %= m_dist2;
                if (m_ib == 0)
                    m_ib = 1;
            }
            m_recorded_ratio = static_cast<float>(m_ia) / static_cast<float>(m_ib);
            return true;
        }
        return false;
    }

    ////////////////////////////////////////////////////////////
    void Rit::swapInternalRatios()
    {
        std::swap(m_dist1, m_dist2);

        m_ia = 1;
        m_ib = 1;
        m_recorded_ratio = static_cast<float>(m_ia) / static_cast<float>(m_ib);
        m_requested_ratio = m_dist1 / static_cast<float>(m_dist2);
    }

} // Namespace ttl
