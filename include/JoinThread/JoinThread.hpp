#ifndef JOINTHREAD_HPP_INCLUDED
#define JOINTHREAD_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <thread>
#include <functional>


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief Scoped thread that joins at scope exit
    ///
    ////////////////////////////////////////////////////////////
    class JoinThread : public std::thread
    {
    public:

        using std::thread::thread;
        ////////////////////////////////////////////////////////////
        /// \brief Constructor delegator
        ///
        /// \see std::thread::thread(...)
        ///
        ////////////////////////////////////////////////////////////
//        JoinThread();

        ////////////////////////////////////////////////////////////
        /// \brief Constructor delegator
        ///
        /// \see std::thread::thread(...)
        ///
        ////////////////////////////////////////////////////////////
//        template <typename ...Args>
//        JoinThread(Args &&...args):std::thread(std::forward<Args>(args)...){}

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Joins the thread
        ///
        ////////////////////////////////////////////////////////////
        ~JoinThread();

    };

} // Namespace ttl

#endif // JOINTHREAD_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class JoinThread
/// \ingroup Utilities
///
/// This class works exactly like std::thread since it
/// inherits therefrom and delegates all constructor
/// arguments to std::thread::thread(...)
///
/// \code
/// int &n = 0;
/// JoinThread thr
/// (
///     [](int &number)
///     {
///         number++;
///         std::this_thread::sleep_for(std::chrono::seconds(3));
///     },
///     std::ref(n)
/// );
/// \endcode
///
/// Don't worry about detaching the thread, a check is
/// performed during destruction to make sure the thread
/// is joinable.
///
/// \see std::thread
///
////////////////////////////////////////////////////////////
