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


#ifndef ARGUMENT_HPP_INCLUDED
#define ARGUMENT_HPP_INCLUDED

// Headers
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <cstddef>


namespace ttl
{
    class Argument
    {
    public:

        Argument();
        Argument(const std::size_t argc, char **argv);
        Argument(const std::string &argument);
        ~Argument();

        void pass(const std::size_t argc, char **argv);
        void pass(const std::string &argument);

        bool isPassed(const std::string &flag) const;
        bool isPassed(const char flag) const;

        const std::string &getArgument(const std::string &flag) const;
        const std::string &getArgument(const char flag) const;

        const std::string &getPath() const;

        void setInert(const std::string &flag);
        void setInert(const char flag);

        bool isInert(const std::string &flag) const;
        bool isInert(const char flag) const;

        const std::string &getOperand(const std::size_t id) const;
        std::size_t getOperandCount() const;

        friend ::std::ostream &operator<<(::std::ostream &out, const ttl::Argument &argument);

    private:

        std::string m_path;
        std::map<std::string, std::string> m_flags_and_parameters;
        std::vector<std::string> m_operands;
        std::set<std::string> m_inert_flags;

        typedef
            std::pair<decltype(m_flags_and_parameters)::iterator, bool>
            InsertReturn;
        typedef
            std::stack<InsertReturn>
            TheUnset;

        void setArgumentOfUnsetUninertFlag
        (
            const std::string &argument,
            TheUnset &unset_flags
        );

        void setSingleCharFlags
        (
            const std::string &argument,
            TheUnset &unset_flags
        );

        std::string parseEqualArgument
        (
            const std::string &argument,
            std::size_t x
        );

        void parseEqualArgument
        (
            const std::string &argument,
            TheUnset &unset_flags
        );

        void setMultiCharFlag
        (
            const std::string &argument,
            TheUnset &unset_flags
        );

    };

} // Namespace ttl

#endif // ARGUMENT_HPP_INCLUDED
