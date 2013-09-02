////////////////////////////////////////////////////////////
//
// TTL - Tea Tank Library
// Copyright (C) 2013 Kevin R. Stravers (macocio@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <fstream>
#include <functional>
#include <iostream>
#include <thread>
#include <sstream>
#include <chrono>
#include <Timestamp/Timestamp.hpp>


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief Simple logging utility
    ///
    ////////////////////////////////////////////////////////////
    template <bool State>
    class Logger
    {};


    ////////////////////////////////////////////////////////////
    /// \brief Simple logging utility - True part
    ///
    /// The true specialized template for Logger is the part that
    /// actually logs when requested to do so. It keeps open an
    /// fstream to the file it is supposed to write to.
    ///
    /// \see Logger<false>
    ///
    ////////////////////////////////////////////////////////////
    template <>
    class Logger<true>
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param filename The file name and location
        /// \param to_clog Whether to output to std::clog ostream or not
        /// \param std_ios_flags The mode to open the file in
        ///
        /// \see std::ios::openmode
        ///
        ////////////////////////////////////////////////////////////
        explicit Logger(const char *filename = "log.log", bool to_clog = true, std::ios::openmode std_ios_flags = std::ios::out | std::ios::app);

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param filename The file name and location
        /// \param to_clog Whether to output to std::clog ostream or not
        /// \param std_ios_flags The mode to open the file in
        ///
        /// \see std::ios::openmode
        ///
        ////////////////////////////////////////////////////////////
        explicit Logger(std::string &filename, bool to_clog = true, std::ios::openmode std_ios_flags = std::ios::out | std::ios::app);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~Logger();

        ////////////////////////////////////////////////////////////
        /// \brief Operator<<
        ///
        /// \param rhs The argument to add to the log
        ///
        /// \see std::ostream::operator<<
        ///
        ////////////////////////////////////////////////////////////
        template <typename T>
        Logger &operator <<(T &&rhs)
        {
            if (clog_it)
            {
                std::clog << rhs/* << std::flush*/;
            }
            output << rhs/* << std::flush*/;
            return std::ref(*this);
        }

        Logger &operator <<(m_Timestamp)
        {
            std::string timestamp = getTimeStamp();
            if (clog_it)
            {
                std::clog << timestamp << ": "/* << std::flush*/;
            }
            output << timestamp << ": "/* << std::flush*/;
            return std::ref(*this);
        }

    private:

        std::ofstream output; ///< The file to output to
        bool clog_it : 1; ///< Whether to output to std::clog as well
    };


    ////////////////////////////////////////////////////////////
    /// \brief Simple logging utility - False part
    ///
    /// The false part is a specialized template that disables
    /// all logger functionality. The compiler is likely to optimize
    /// away the empty function calls resulting in an object that
    /// literally does nothing in running code. There is also
    /// no data usage at all.
    ///
    /// \see Logger<true>
    ///
    ////////////////////////////////////////////////////////////
    template <>
    class Logger<false>
    {
    public:

        explicit Logger(const char *filename = "log.log", bool to_clog = true, std::ios::openmode std_ios_flags = std::ios::out | std::ios::app);
        explicit Logger(std::string &filename, bool to_clog = true, std::ios::openmode std_ios_flags = std::ios::out | std::ios::app);
        ~Logger();

        template <typename T>
        Logger &operator <<(T &&rhs)
        {
            return std::ref(*this);
        }
    };

    ////////////////////////////////////////////////////////////
    /// \class Logger
    /// \ingroup Programming Utilities
    ///
    /// This class will log to a file with an optional timestamp
    /// and other data. Changing the template argument turns the
    /// logger on and off, thus allowing the programmer to turn
    /// off the logger by changing a single argument, whilst
    /// leaving the logging code intact. This allows for easy
    /// debugging and information fetching.
    ///
    /// \code
    /// Logger<true> log("messages.log");
    /// log << "A message";
    /// \endcode
    ///
    ////////////////////////////////////////////////////////////

} // Namespace ttl

#endif // LOGGER_HPP_INCLUDED
