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


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ScopedFunction/ScopedFunction.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction()
    :
        fn([](){})
    {}

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction(const ScopedFunction &scopedfunction)
    :
        fn(scopedfunction.fn)
    {
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction(ScopedFunction &&scopedfunction)
    :
        fn(scopedfunction.fn)
    {
        scopedfunction.fn = [](){};
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction &ScopedFunction::operator=(const ScopedFunction &scopedfunction)
    {
        fn = scopedfunction.fn;
        return *this;
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction &ScopedFunction::operator=(ScopedFunction &&scopedfunction)
    {
        fn = scopedfunction.fn;
        scopedfunction.fn = [](){};
        return *this;
    }

    ////////////////////////////////////////////////////////////
    void ScopedFunction::reset()
    {
        fn = [](){};
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction::~ScopedFunction()
    {
        fn();
    }

} // Namespace ttl
