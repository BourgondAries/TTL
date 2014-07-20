/*
Copyright 2013, 2014 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

// Headers
#include <fstream>
#include <functional>
#include <iostream>
#include <thread>
#include <sstream>
#include <chrono>
#include <../include/Timestamp/Timestamp.hpp>


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

} // Namespace ttl

#endif // LOGGER_HPP_INCLUDED


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
