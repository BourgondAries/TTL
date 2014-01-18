/*
Copyright 2013 Kevin Robert Stravers

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


#ifndef RIT_HPP_INCLUDED
#define RIT_HPP_INCLUDED

// Headers
#include "Ttldef/Ttldef.hpp"


namespace ttl
{
    ////////////////////////////////////////////////////////////
    /// \brief Allows binary distribution of iterative functions.
    ///
    ////////////////////////////////////////////////////////////
    class Rit
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// Standard constructor that uses a 1/1 distribution.
        ///
        ////////////////////////////////////////////////////////////
        Rit();

        ////////////////////////////////////////////////////////////
        /// \brief Constructor with rationals
        ///
        /// Constructor that uses a dist1/dist2 distribution.
        ///
        ////////////////////////////////////////////////////////////
        Rit(const Sti_t distribution_1, const Sti_t distribution_2);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destructor.
        ///
        ////////////////////////////////////////////////////////////
        ~Rit();

        ////////////////////////////////////////////////////////////
        /// \brief Set the distribution
        ///
        /// \param distribution_1 distribution for First function
        /// \param distribution_2 distribution for Second function
        ///
        ////////////////////////////////////////////////////////////
        void setDistribution(const Sti_t distribution_1, const Sti_t distribution_2);

        ////////////////////////////////////////////////////////////
        /// \brief Gets the set distribution
        ///
        /// \return the First distribution.
        ///
        ////////////////////////////////////////////////////////////
        Sti_t getFirstDistribution() const;

        ////////////////////////////////////////////////////////////
        /// \brief Gets the set distribution
        ///
        /// \return the Second distribution.
        ///
        ////////////////////////////////////////////////////////////
        Sti_t getSecondDistribution() const;

        ////////////////////////////////////////////////////////////
        /// \brief Gets the set ratio
        ///
        /// \return the ratio.
        ///
        ////////////////////////////////////////////////////////////
        float getRatio() const;

        ////////////////////////////////////////////////////////////
        /// \brief First function
        ///
        /// \return whether it is valid to enter the first function.
        ///
        ////////////////////////////////////////////////////////////
        bool isFirstReady();

        ////////////////////////////////////////////////////////////
        /// \brief Second function
        ///
        /// \return whether it is valid to enter the second function.
        ///
        ////////////////////////////////////////////////////////////
        bool isSecondReady();

        void swapInternalRatios();

    private:

        Sti_t m_dist1, m_dist2; ///< Ratio given by a rational
        float m_requested_ratio; ///< The ratio requested by the user
        float m_recorded_ratio; ///< The ratio that is being recorded
        Sti_t m_ia, m_ib; ///< Iteration counters

    };

} // Namespace ttl

#endif // RIT_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Rit
///
/// Class used for managing 2 loops. One can run say at 60 ips,
/// whilst the other runs at 15 ips. It will alternate between
/// the two in such a way that it accomplishes this exact
/// distribution.
///
/// \code
/// ttl::Rit rit(1, 10);
/// rit.setDistribution(23, 32);
/// Sti_t log(0), ren(0);
/// for (int i = 0; i < 50000000; ++i)
/// {
///     while (rit.isFirstReady())
///     {
///         ++log;
///     }
///     while (rit.isSecondReady())
///     {
///         ++ren;
///     }
/// }
///
/// std::cout << "For 23 of first, run 32 of second: " << "\nfirst: " << log << "\nsecond: " << ren << "\n(First / 23) * 32: " << 32 * (log / static_cast<float>(23)) << std::endl;
/// std::cout << "Ratio: should be 23/32 = " << log / static_cast<float>(ren) << std::endl;
/// \endcode
///
/// Note: The isFirstReady and isSecondReady methods increment
/// their internal counters whilst returning a true.
/// If they return a false, the counter is not updated.
///
/// Another use may be to use this class in a counter of some
/// sort. Say we want a sprite to flicker for 10 iterations,
/// whilst showing itself for 10 iterations afterwards.
///
/// \code
/// if (r.isFirstReady())
/// {
///     if (!b.fetch_and_enable())
///         r.swapInternalRatios();
/// }
/// if (r.isSecondReady())
/// {
///     if (b.fetch_and_disable())
///         r.swapInternalRatios();
/// }
/// \endcode
///
/// Where r is a ttl::Rit(10, 1) object, and b is a
/// ttl::Bool(true) object. Putting these in a Mixin
/// is a very clean solution:
/// ttl::Mixin<ttl::Bool, ttl::Rit> persisted_distribution;
/// auto &x = persisted_distribution;
/// x.ttl::Bool::fetch_and_set(true);
/// x.ttl::Rit::setDistribution(10, 1);
///
/// Or one can use a struct:
/// struct {ttl::Rit r; ttl::Bool b} persisted_distribution;
////////////////////////////////////////////////////////////
