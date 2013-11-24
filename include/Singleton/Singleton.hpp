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


#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED


namespace ttl
{

    /**
    * \brief Generic singleton class
    *
    * A singleton is an object that exists globally.
    * The singleton can not be instantiated nor destroyed
    * during program execution; but is instead created
    * before main starts and destroyed after it returns.
    *
    * This class allows the usage of singletons in
    * even non-singleton classes..
    */
    template <typename CONTAINS, int ID = 0>
    class Singleton
    {
    public:

        /**
        * \brief Deleted default constructor
        *
        * A singleton can not be instantiated locally
        */
        Singleton() = delete;

        /**
        * \brief Grabs the reference to the singleton class with the specified ID
        * \return The Singleton in the Singleton-ID pair
        */
        static CONTAINS &get() {return Singleton<CONTAINS, ID>::m_content;}

    private:

        static CONTAINS m_content; ///< The actual data

    };

    template <typename CONTAINS, int ID>
    CONTAINS Singleton<CONTAINS, ID>::m_content;

} // Namespace ttl

#endif // SINGLETON_HPP_INCLUDED


/**
* \class Singleton
* \ingroup Programming Utilities
*
* Here is a code sample
* \code
* Singleton<int, 42>::get() = 19;
* Singleton<int, 0>::get() = 1232; // 0 can be omitted
* Singleton<float, 42>::get() = 2.509f;
* // From different location in the code (both r/w); operations are possible:
* std::cout << Singleton<int, 42>::get() << "\n";
* std::cout << Singleton<int>::get() << "\n"; // Basically Singleton<int, 0>
* std::cout << Singleton<float, 42>::get() << "std::endl";
* \endcode
*/
