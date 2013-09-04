////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <thread>
#include <chrono>
#include <cstddef>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    void sleep(const std::size_t sec)
    {
        std::this_thread::sleep_for(std::chrono::seconds(sec));
    }

    ////////////////////////////////////////////////////////////
    void msleep(const std::size_t msec)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

    ////////////////////////////////////////////////////////////
    void usleep(const std::size_t usec)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(usec));
    }

} // Namespace ttl
