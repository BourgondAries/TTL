#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include "Synched/Synched.hpp"
#include "Valman/Valman.hpp"

namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief a convenience function to read from synched valmans
    ///
    /// \param arg The synched value manager to read from
    /// \param key The index to retrieve data from
    /// \return A string containing the data
    ///
    ////////////////////////////////////////////////////////////
    std::string read(const Synched<Valman> &arg, const std::string &key);

    ////////////////////////////////////////////////////////////
    /// \brief a convenience function to read from synched valmans
    ///
    /// This function also checks for the key's validity
    /// The check makes this function slower, but safer.
    ///
    /// \param arg The synched value manager to read from
    /// \param key The index to retrieve data from
    /// \return A string containing the data
    ///
    ////////////////////////////////////////////////////////////
    std::string sread(const Synched<Valman> &arg, const std::string &key);

}

#endif // UTILITIES_HPP_INCLUDED
