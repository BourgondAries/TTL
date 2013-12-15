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
along with schdl.  If not, see <http://www.gnu.org/licenses/>.
*/


// Headers
#include "Argument/Argument.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    void Argument::setInert(const std::string &flag)
    {
        const sti sz = flag.size();
        if (sz > 0)
        {
            if (flag[0] == '-')
            {
                if (sz > 1 && flag[1] == '-')
                {
                    m_inert_flags.insert(flag);
                }
                else
                {
                    for (sti i = 1; i < sz; ++i)
                    {
                        m_inert_flags.insert(std::string("-") + flag[i]);
                    }
                }
            }
        }

    }

    ////////////////////////////////////////////////////////////
    void Argument::setInert(const char flag)
    {
        std::string tmp("-");
        tmp.push_back(flag);
        this->setInert(tmp);
    }

} // Namespace ttl
