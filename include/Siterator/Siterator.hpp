/*
Copyright 2013, 2014-2014 Kevin Robert Stravers

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

#ifndef SITERATOR_HPP_INCLUDED
#define SITERATOR_HPP_INCLUDED

// Headers
#include <iterator>
#include <utility>
#include <TTL/Ttldef/Ttldef.hpp>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Generic random_access_Siterator_tag compliant Siterator
    ///
    /// This Siterator can wrap around any type so long the
    /// following operations are supported:
    ///
    /// Constructor:
    /// Default Constructor
    /// Copy Constructor
    /// Move Constructor
    ///
    /// Assignment:
    /// Copy Assignment
    /// Move Assignment
    ///
    /// operator==
    /// prefix operator++
    /// prefix operator--
    /// operator<
    /// operator+=
    /// operator-=
    ///
    /// Many other operators, like operator!= are defined
    /// by using one or more of the above operators. If
    /// your template argument is a class, then you only
    /// need to define 11 operators (of the above) to get
    /// a fully ::std:: compliant random_access_Siterator.
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    class Siterator
    :
        public std::iterator<std::random_access_iterator_tag, T, T>
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Call T's default constructor.
        ///
        ////////////////////////////////////////////////////////////
        Siterator() = default;

        ////////////////////////////////////////////////////////////
        /// \brief Call T's copy constructor.
        ///
        /// Copies it's private T value into this T.
        /// \param it The Siterator to copy from.
        ///
        ////////////////////////////////////////////////////////////
        Siterator(const Siterator &it)
        :
            m_val(it.m_val) {}

        ////////////////////////////////////////////////////////////
        /// \brief Call T's move constructor.
        ///
        /// Moves one T to another.
        /// \param it The Siterator to move T from.
        ///
        ////////////////////////////////////////////////////////////
        Siterator(Siterator &&it)
        :
            m_val(std::move(it.m_val)) {}

        ////////////////////////////////////////////////////////////
        /// \brief Call T's copy constructor.
        ///
        ////////////////////////////////////////////////////////////
        Siterator(const T &t)
        :
            m_val(t) {}

        ////////////////////////////////////////////////////////////
        /// \brief Call T's move constructor.
        ///
        ////////////////////////////////////////////////////////////
        Siterator(T &&t)
        :
            m_val(std::move(t)) {}

        ////////////////////////////////////////////////////////////
        /// \brief Default destructor.
        ///
        ////////////////////////////////////////////////////////////
        ~Siterator() = default;

        ////////////////////////////////////////////////////////////
        /// \brief Copy assignment operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator=(const Siterator &it)
        {
            m_val = it.m_val;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Move assignment operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator=(Siterator &&it)
        {
            m_val = std::move(it.m_val);
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Copy assignment operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator=(const T &t)
        {
            m_val = t;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Equality operator.
        ///
        ////////////////////////////////////////////////////////////
        bool operator==(const Siterator &it)
        {
            return m_val == it.m_val;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Inequality operator.
        ///
        ////////////////////////////////////////////////////////////
        bool operator!=(const Siterator &it)
        {
            return !(m_val == it.m_val);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Dereference operator.
        ///
        /// Normally, an Siterator's operator* returns a reference
        /// to the object that the Siterator is pointing towards.
        /// In this case; it is simply the value the Siterator
        /// is holding at that time.
        ///
        /// \return A reference of the internal value of T.
        ///
        ////////////////////////////////////////////////////////////
        T &operator*()
        {
            return m_val;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Pointerize operator.
        ///
        /// \return a pointer to the T object.
        ///
        ////////////////////////////////////////////////////////////
        T *operator->()
        {
            return &m_val;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Prefix increment operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator++()
        {
            ++m_val;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Postfix increment operator.
        ///
        /// Defined by the prefix increment operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator operator++(int)
        {
            Siterator tmp(m_val);
            ++(*this);
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Prefix decrement operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator--()
        {
            --m_val;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Postfix decrement operator.
        ///
        /// Defined by the prefix decrement operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator operator--(int)
        {
            Siterator tmp(m_val);
            --(*this);
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Addition operator.
        ///
        /// Defined by the operators copy constructor and +=.
        ///
        ////////////////////////////////////////////////////////////
        Siterator operator+(const Siterator &it)
        {
            T tmp(m_val);
            tmp += it;
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Addition operator.
        ///
        /// Adds a raw value to T.
        ///
        ////////////////////////////////////////////////////////////
        Siterator operator+(const T &n)
        {
            Siterator tmp(m_val);
            tmp += n;
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Subtraction operator.
        ///
        /// \return The raw value, as specified by the standard.
        ///
        ////////////////////////////////////////////////////////////
        T operator-(const Siterator &it)
        {
            T tmp(m_val);
            tmp -= it.m_val;
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Call T's default constructor.
        ///
        /// \return The raw value, as specified by the standard.
        ///
        ////////////////////////////////////////////////////////////
        T operator-(const T &n)
        {
            T tmp(m_val);
            tmp -= n;
            return tmp;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Less-than boolean operator.
        ///
        /// \return whether lhs is less than rhs.
        ///
        ////////////////////////////////////////////////////////////
        bool operator<(const Siterator& it) const
        {
            return m_val < it.m_val;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Larger-than boolean operator.
        ///
        /// \return whether lhs is larger than rhs.
        ///
        ////////////////////////////////////////////////////////////
        bool operator>(const Siterator& it) const
        {
            return !((*this) <= it);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Less-than or equal boolean operator.
        ///
        /// \return whether lhs is less than or equal to rhs.
        ///
        ////////////////////////////////////////////////////////////
        bool operator<=(const Siterator& it) const
        {
            return m_val < it.m_val || m_val == it.m_val;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Larger-than or equal boolean operator.
        ///
        /// \return whether lhs is larger than or equal to rhs.
        ///
        ////////////////////////////////////////////////////////////
        bool operator>=(const Siterator& it) const
        {
            return !((*this) < it);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Forward motion operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator+=(const Siterator &it)
        {
            m_val += it.m_val;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Forward motion operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator+=(const T &n)
        {
            m_val += n;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Backward motion operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator-=(const Siterator &it)
        {
            m_val -= it.m_val;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Backward motion operator.
        ///
        ////////////////////////////////////////////////////////////
        Siterator &operator-=(const T &n)
        {
            m_val -= n;
            return *this;
        }

        ////////////////////////////////////////////////////////////
        /// \brief Array index operator.
        ///
        /// Normally, on an array or vector, this function returns
        /// a reference to the object. In this case, it will add
        /// a value to the object and return that value instead,
        /// because it is assumed that T is not an array, but
        /// a single object.
        ///
        ////////////////////////////////////////////////////////////
        T operator[](const T &pos) const
        {
            return m_val + pos;
        }


    private:

        T m_val;
    };

    ////////////////////////////////////////////////////////////
    /// \brief Addition of a raw value before the Siterator<T>.
    ///
    /// E.g.: Siterator<int> nit = 5 + it;
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    Siterator<T> operator+(const T &n, const Siterator<T> &it)
    {
        return Siterator<T>(n + it[0]);
    }

    ////////////////////////////////////////////////////////////
    /// \brief Subtraction of a raw value before the Siterator<T>.
    ///
    /// E.g.: Siterator<int> nit = 5 - it;
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    Siterator<T> operator-(const T &n, const Siterator<T> &it)
    {
        return Siterator<T>(n - it[0]);
    }

    typedef Siterator<Sti_t> Sit; ///< A typedef for the most common Siterator, of std::size_t: the largest indicible value.

}


#endif // SITERATOR_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// \class
///
/// Siterator allows you to define an arbitrary, single-
/// object iterator that is ::std compliant.
///
/// An example is:
/// \code
/// std::for_each(ttl::Sit(0), ttl::Sit(33), [](const std::size_t &x){std::cout << x << std::endl;});
/// \endcode
///
/// In this example, we can not use raw integers to
/// iterate like this, surely we can use a normal
/// for. It can be useful to use Sit with
/// generic iterator algorithms.
///
////////////////////////////////////////////////////////////
