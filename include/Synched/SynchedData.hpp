#ifndef SYNCHEDDATA_HPP_INCLUDED
#define SYNCHEDDATA_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <mutex>
#include <atomic>
#include "Flare/Flare.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Wrapper of Synched's internal data
    ///
    ////////////////////////////////////////////////////////////
    struct SynchedData
    {
        SynchedData();
        ~SynchedData() = default;

        std::mutex  entry_mutex;
        ttl::Flare  writer_activation;
        std::atomic<std::size_t> readers;
    };

} // Namespace ttl

#endif // SYNCHEDDATA_HPP_INCLUDED
