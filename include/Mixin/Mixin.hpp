#ifndef MIXIN_HPP_INCLUDED
#define MIXIN_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief A class for mixing classes with eachother
    ///
    ////////////////////////////////////////////////////////////
    template <typename ...CLASSES>
    class Mixin : public CLASSES...
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// Using template arguments, constructs the classes
        ///
        ////////////////////////////////////////////////////////////
        explicit Mixin()
        :
        CLASSES()...
        {}

    //    ////////////////////////////////////////////////////////////
    //    /// \brief Constructor
    //    ///
    //    /// Supposed to work with arguments,... unfinished.
    //    ///
    //    ////////////////////////////////////////////////////////////
    //    template <typename ...Args>
    //    Mixin(Args &&...args)
    //    :
    //    CLASSES(std::forward<Args>(args))...
    //    {}
    };

} // Namespace ttl

#endif // MIXIN_HPP_INCLUDED
