#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <exception>
#include <string>


#ifdef NDEBUG
    #define TTL_DEBUG_THROW(x)
    #define TTL_DEBUG_IF(x)
    #define TTL_DEBUG_ELSE
    #define TTL_DEBUG_SCOPE(x)
#else
    #define TTL_DEBUG_THROW(x)                          \
        throw std::runtime_error                        \
        (                                               \
            std::string("START OF EXCEPTION MESSAGE")   \
            + "\nMessage: "                             \
            + x                                         \
            + "\nFile: "                                \
            + __FILE__                                  \
            + "\nFunction: "                            \
            + __func__                                  \
            + "\nLine: "                                \
            + std::to_string(__LINE__)                  \
            + std::string("\nEND OF EXCEPTION MESSAGE")    \
        )
    #define TTL_DEBUG_IF(x) if(x)
    #define TTL_DEBUG_ELSE else
    #define TTL_DEBUG_SCOPE(x) x
#endif // NDEBUG


#endif // DEBUG_HPP_INCLUDED
