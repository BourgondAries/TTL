////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Rit/Rit.hpp"
#include <limits>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Rit::Rit()
    :
        m_dist1(1),
        m_dist2(1),
        m_requested_ratio(1 / static_cast<float>(1)),
        m_recorded_ratio(0.f),
        m_ia(0),
        m_ib(1)
    {}

    ////////////////////////////////////////////////////////////
    Rit::Rit(const std::size_t distribution_1, const std::size_t distribution_2)
    :
        m_dist1(distribution_1),
        m_dist2(distribution_2),
        m_requested_ratio(distribution_1 / static_cast<float>(distribution_2)),
        m_recorded_ratio(0.f),
        m_ia(0),
        m_ib(1)
    {}

    ////////////////////////////////////////////////////////////
    Rit::~Rit(){}

    ////////////////////////////////////////////////////////////
    void Rit::setDistribution(const std::size_t distribution_1, const std::size_t distribution_2)
    {
        m_dist1 = distribution_1;
        m_dist2 = distribution_2;
        m_requested_ratio = distribution_1 / static_cast<float>(distribution_2);
        m_recorded_ratio = 0.f;
        m_ia = 0;
        m_ia = 1;
    }

    ////////////////////////////////////////////////////////////
    std::size_t Rit::getFirstDistribution() const
    {
        return m_dist1;
    }

    ////////////////////////////////////////////////////////////
    std::size_t Rit::getSecondDistribution() const
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
        if (m_recorded_ratio < m_requested_ratio)
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

} // Namespace ttl
