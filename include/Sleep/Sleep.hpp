#ifndef SLEEP_HPP_INCLUDED
#define SLEEP_HPP_INCLUDED

#include <cstddef>

namespace ttl
{

    extern void sleep(std::size_t sec);
    extern void msleep(std::size_t msec);
    extern void usleep(std::size_t usec);

} // Namespace ttl

#endif // SLEEP_HPP_INCLUDED
