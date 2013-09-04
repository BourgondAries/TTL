#ifndef RUNNABLE_HPP_INCLUDED
#define RUNNABLE_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <memory>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Class for managing cycles of runnable objects
    ///
    /// Inheriting from this class and overriding its run function
    /// will allow you to send such class into ttl::Runnable::cycle
    ///
    ////////////////////////////////////////////////////////////
    class Runnable
    {
    public:

        Runnable() = default;
        virtual ~Runnable() = default;

        virtual Runnable *run() = 0;

        ////////////////////////////////////////////////////////////
        /// \brief Handles exceptions and logs times.
        ///
        ////////////////////////////////////////////////////////////
        static void cycle(std::unique_ptr<Runnable> runnable);
    };

} // Namespace ttl

#endif // RUNNABLE_HPP_INCLUDED
