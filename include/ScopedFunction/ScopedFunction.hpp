#ifndef SCOPEDFUNCTION_HPP_INCLUDED
#define SCOPEDFUNCTION_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Executes the function upon scope exit
    ///
    /// This RAII wrapper of functions executes a function on
    /// scope exit. This can be used ensure exception safety.
    ///
    ////////////////////////////////////////////////////////////
    class ScopedFunction
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Empty Constructor
        ///
        /// Will construct an empty function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction();

        ////////////////////////////////////////////////////////////
        /// \brief Copy Constructor
        ///
        /// Will copy the function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction(const ScopedFunction &scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Move Constructor
        ///
        /// Will copy the function.
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction(ScopedFunction &&scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Constructor that takes a function
        ///
        /// Can take bind expressions as well. Function is executed
        /// at scope exit unless reset.
        ///
        /// \param args Arguments to the bind expression.
        ///
        ////////////////////////////////////////////////////////////
        template <typename ...Args>
        ScopedFunction(Args &&...args)
        {
            auto f = std::bind(std::forward<Args>(args)...);
            fn = [=]()
            {
                f();
            };
        }

        ////////////////////////////////////////////////////////////
        /// \brief Copy Assignment
        ///
        /// \param scopedfunction The ScopedFunction to copy
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction &operator=(const ScopedFunction &scopedfunction);

        ////////////////////////////////////////////////////////////
        /// \brief Move Assignment
        ///
        /// \param scopedfunction The ScopedFunction to move from
        ///
        ////////////////////////////////////////////////////////////
        ScopedFunction &operator=(ScopedFunction &&scopedfunction);

        void reset();

        template <typename ...Args>
        void reset(Args &&...args)
        {
            auto f = std::bind(std::forward<Args>(args)...);
            fn = [=]()
            {
                f();
            };
        }

        ~ScopedFunction();

    private:

        std::function<void()> fn;

    };

} // Namespace ttl


#endif // SCOPEDFUNCTION_HPP_INCLUDED
