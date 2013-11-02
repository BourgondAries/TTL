#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <exception>
#include <string>


#ifdef NDEBUG
    #define dthrow(x) ((void)0)
    #define dif(x) ((void)0);
#else
    #define dthrow(x)   \
        throw std::runtime_error \
        (   \
            std::string("Exception")    \
            + "\nMessage: "    \
            + x \
            + "\nFile: "   \
            + __FILE__ \
            + "\nFunction: "    \
            + __func__  \
            + "\nLine: "    \
            + std::to_string(__LINE__)  \
        )
    #define dif(x) if(x)
#endif // NDEBUG

#endif // DEBUG_HPP_INCLUDED
