#ifndef TIMESTAMP_HPP_INCLUDED
#define TIMESTAMP_HPP_INCLUDED

////////////////////////////////////////////////////////////
#include <string>
#include <chrono>
#include <ctime>

namespace ttl
{

    std::string getTimeStamp();
    struct m_Timestamp {} static constexpr const Timestamp = m_Timestamp();

} // Namespace ttl

#endif // TIMESTAMP_HPP_INCLUDED
