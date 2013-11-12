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


#include "Utilities/Utilities.hpp"

namespace ttl
{

    ////////////////////////////////////////////////////////////
    std::string read(const Synched<Valman> &arg, const std::string &key)
    {
        auto x = arg.getReadAccess();
        std::unordered_map<std::string, std::string>::const_iterator derp = x->find(key);
        return derp->second;
    }

    ////////////////////////////////////////////////////////////
    std::string sread(const Synched<Valman> &arg, const std::string &key)
    {
        auto x = arg.getReadAccess();
        std::unordered_map<std::string, std::string>::const_iterator derp = x->find(key);
        if (derp != x->end())
            return derp->second;
        else
            return "";
    }

    ////////////////////////////////////////////////////////////
    std::string tread(const Synched<Valman> &arg, const std::string &key)
    {
        auto x = arg.getReadAccess();
        std::unordered_map<std::string, std::string>::const_iterator derp = x->find(key);
        if (derp != x->end())
            return derp->second;
        else
            throw std::runtime_error("tread(\"" + key + "\")");
    }

}
