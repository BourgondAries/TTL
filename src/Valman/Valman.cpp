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


// Headers
#include "Valman/Valman.hpp"


namespace ttl
{
    ////////////////////////////////////////////////////////////
    // Standard member functions
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    Valman::Valman()
    {}

    ////////////////////////////////////////////////////////////
    Valman::Valman(const std::string &filename)
    {
        load(filename);
    }

    ////////////////////////////////////////////////////////////
    Valman::~Valman(){}

    ////////////////////////////////////////////////////////////
    // Overloaded access operators
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    std::string &Valman::at(const std::string &str)
    {
        // Check the validity of the m_commandument
        decltype(m_registry)::iterator it = m_registry.find(str);
        if (it == m_registry.end())
            throw std::invalid_argument("Valman::at(\"" + str + "\")");
        return it->second;
    }

    ////////////////////////////////////////////////////////////
    std::string &Valman::operator[](const std::string &str)
    {
        return m_registry[str];
    }

    ////////////////////////////////////////////////////////////
    // Utility functions
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    void Valman::clear()
    {
        m_registry.clear();
    }

    ////////////////////////////////////////////////////////////
    bool Valman::load(const std::string &filename)
    {
        // Generic loader from file:
        std::fstream file(filename.data(), std::ios::in);

        if (file.is_open())
        {
            std::pair<std::string, std::string> pr;
            while (!file.eof())
            {
                file >> pr.first;
                if (pr.first == "")
                {
                    continue;
                }
                file >> pr.second;
                m_registry.insert(pr);
            }
            return true; // Notify the caller that reading succeeded
        }
        return false;
    }

    ////////////////////////////////////////////////////////////
    void Valman::add(const std::pair<std::string, std::string> &value)
    {
        m_registry.insert(value);
    }

    ////////////////////////////////////////////////////////////
    void Valman::add(const std::string &m_command)
    {
        m_registry.insert(std::make_pair(m_command, ""));
    }

    ////////////////////////////////////////////////////////////
    void Valman::erase(const std::string &entry)
    {
        m_registry.erase(entry);
    }

    ////////////////////////////////////////////////////////////
    void Valman::store(const std::string &filename)
    {
        std::fstream file(filename, std::ios::out | std::ios::trunc);

        for (auto &x : m_registry)
        {
            file
                << x.first
                << " "
                << x.second
                << "\n";
        }
    }

    ////////////////////////////////////////////////////////////
    const std::unordered_map<std::string, std::string>::const_iterator Valman::find(const std::string &key) const
    {
        return m_registry.find(key);
    }

    ////////////////////////////////////////////////////////////
    const std::unordered_map<std::string, std::string>::const_iterator Valman::end() const
    {
        return m_registry.end();
    }

    ////////////////////////////////////////////////////////////
    void Valman::edit()
    {
        std::cout
            << "------------------ Valman v0.6 ------------------"
            << std::endl
            << "-    Value manager and loader, \".h\" for help    -"
            << std::endl
            << "-------------------------------------------------"
            << std::endl << std::endl;

        bool running = true;

        Editor editor(this, std::cout);

        while (running)
        {
            editor.prompt();
            editor.event(running);
            std::cout
                << std::endl
                << std::endl;
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::edit(const std::string &command)
    {
        bool running = true;

        Editor editor(this, std::cout);
        editor.m_command = command;

        editor.event(running);
        std::cout
            << std::endl
            << std::endl;
        return;
    }

    ////////////////////////////////////////////////////////////
    std::stringstream &Valman::edit(const std::string &command, std::stringstream &output)
    {
        output.str("");
        bool running = true;

        Editor editor(this, output);
        editor.m_command = command;

        editor.event(running);
        output
            << std::endl
            << std::endl;
        return output;
    }

    ////////////////////////////////////////////////////////////
    std::ostream &Valman::edit(const std::string &command, std::ostream &output)
    {
        bool running = true;

        Editor editor(this, output);
        editor.m_command = command;

        editor.event(running);
        output
            << std::endl
            << std::endl;
        return output;
    }

    ////////////////////////////////////////////////////////////
    // Private subroutines
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
//    bool Valman::isOperator(const char in)
//    {
//        switch (in)
//        {
//            case '*':
//            case '+':
//            case '-':
//            case '/':
//            case '%':
//                return true;
//            default:
//                return false;
//        }
//    }

    ////////////////////////////////////////////////////////////
    bool Valman::Editor::autoComplete(std::string &to_complete) // Accepts a string and tries to match it
    {
        std::string buffer;

        bool at_least_one_entry = false;

        for (auto &x : m_ptr->m_registry)
        {
            if (x.first.find(to_complete) == 0)
            {
                if (at_least_one_entry)
                {
                    m_streamref << "Too many entries to deduce parameter: " << std::endl;
                    at_least_one_entry = false;
                    for (auto &x : m_ptr->m_registry)
                    {
                        if (x.first.find(to_complete) == 0)
                        {
                            if (at_least_one_entry)
                            {
                                m_streamref << std::endl;
                            }
                            m_streamref << x.first;
                            at_least_one_entry = true;
                        }
                    }
                    return false;
                }
                buffer = x.first;
                at_least_one_entry = true;
            }
        }

        if (at_least_one_entry == false)
        {
            m_streamref << "No entries found that match the criteria";
            return false;
        }

        to_complete = buffer;

        return true;
    }

    ////////////////////////////////////////////////////////////
    bool Valman::Editor::trueAutoComplete()
    {
        // First, find if || occurs, if it does not, return true (We're done)
        // return false is for /autoCompletes that can not be interpreted(too many entries, for example)/
        while (m_command.find(shortcut) != std::string::npos)
        {
            // Create a temporary with only this object:
            std::string pure;
            std::string::iterator insert_location(m_command.begin());

            for (signed int i(static_cast<signed int>(std::distance(m_command.begin(), m_command.begin() + m_command.find_first_of(shortcut)))); i >= 0; --i)
            {
                if (m_command[i] == ' '/* || isOperator(m_command[i])*/) // If there's a space, or a mathematical operator
                {
                    // We copy all m_command from the start of the word up til ||:
                    insert_location = m_command.begin() + i + 1;
                    pure.append(m_command.begin() + i + 1, m_command.begin() + m_command.find_first_of(shortcut));
                    m_command.erase(m_command.begin() + i + 1, m_command.begin() + m_command.find_first_of(shortcut));
                    break;
                }
            }
            if (pure.empty()) // When there was no ' ' before the ||; means that something|| xxxx|| xxx 32 xxxx
            {
                pure.append(m_command.begin(),m_command.begin() + m_command.find_first_of(shortcut));
                m_command.erase(m_command.begin(), m_command.begin() + m_command.find_first_of(shortcut));
            }
    //        m_streamref << "The interpreted m_command: *" << pure << "*" << std::endl;

            // We can now autocomplete it!
            if (autoComplete(pure) == false) // No need for writing the code, again!
            {
                return false;
            }

            // We need to move the result into the m_command:
            m_command.insert(insert_location, pure.begin(), pure.end());

            // Then we remove our first shortcut ||
            std::size_t first_of(m_command.find_first_of(shortcut));
            m_command.erase(m_command.begin() + first_of, m_command.begin() + first_of + 2);
        }

        return true;
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::clear()
    {
        if (m_ptr->m_registry.size() > 0)
        {
            m_ptr->m_registry.clear();
            m_streamref << "All entries cleared";
        }
        else
        {
            m_streamref << "Registry is already empty";
        }
    }


    ////////////////////////////////////////////////////////////
    // Interface subroutines
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceLoad()
    {
        std::string file;
        file.resize(m_command.size() - 6);
        std::copy(m_command.begin() + 6, m_command.end(), file.begin());

        if (m_ptr->load(file))
        {
            m_streamref
                << "Data loaded";
        }
        else
        {
            m_streamref
                << "Unable to load data";
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceAdd()
    {
        // Removes ".add "
        m_command.erase(m_command.begin(), m_command.begin() + 5);
        // Replace whitespaces...
        m_command.erase(0, std::distance(m_command.begin(), std::find_if(m_command.begin(), m_command.end(), [](const char &x)-> bool {return x != ' ';})));
        std::string key = m_command.substr(0, m_command.find(' '));
        m_command.erase(0, key.size() + 1);

        // replace all whitespaces
        std::replace_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x == ' ';}, '_');

        m_ptr->m_registry.insert(std::make_pair(key, m_command));

        m_streamref
            << "The data is added";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceErase()
    {
        m_command.erase(m_command.begin(), std::find_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x == ' ';}));
        m_command.erase(m_command.begin(), std::find_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x != ' ';}));

        if (m_command.size() == 0) // If no m_command is given, delete active
        {
            if (m_last == m_ptr->m_registry.end())
            {
                m_streamref << "No active register to erase";
                return;
            }
            m_streamref << "Active register unset" << std::endl;
            m_ptr->m_registry.erase(m_last);
            m_last = m_ptr->m_registry.end();
        }
        else if (m_ptr->m_registry.find(m_command) == m_ptr->m_registry.end())  // m_command given, does it exist?
        {
            m_streamref << "The entry was not found";
            return;
        }
        else if (m_last == m_ptr->m_registry.find(m_command)) // m_command given, is it itself?
        {
            m_streamref << "Active register unset" << std::endl;
            m_ptr->m_registry.erase(m_command);
            m_last = m_ptr->m_registry.end();
        }
        else // m_command given, validated, not itself
        {
            m_ptr->m_registry.erase(m_command);
        }

        m_streamref << "The data is erased";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceStore()
    {
        m_command.erase(m_command.begin(), std::find_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x == ' ';}));
        m_command.erase(m_command.begin(), std::find_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x != ' ';}));

        m_ptr->store(m_command);

        m_streamref
            << "The data is stored";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceReg()
    {
        m_command.erase(m_command.begin(), m_command.begin() + 5);

        if (m_command.size() == 0)
        {
            m_streamref << "No register specified";
            return;
        }
        else
        {
            decltype(m_last) it = m_ptr->m_registry.find(m_command);
            if (it != m_ptr->m_registry.end())
            {
                m_last = it;
                m_streamref << "Active register: " << it->first;
            }
            else
            {
                m_streamref << "Unable to locate register";
                return;
            }
        }

    }

    ////////////////////////////////////////////////////////////
    // Core UI
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    void Valman::Editor::change()
    {
        if (m_last != m_ptr->m_registry.end())
        {
            std::replace_if(m_command.begin(), m_command.end(), [](const char &x) -> bool {return x == ' ';}, '_');
            m_streamref << "old \"" << m_last->first << "\" = \"" << m_last->second << "\"\n";
            m_last->second = m_command;
            m_streamref << "new \"" << m_last->first << "\" = \"" << m_last->second << "\"";
        }
        else
        {
            m_streamref << "No active register to edit";
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::prompt()
    {
        m_streamref << ">" << std::flush;
        std::getline(std::cin, m_command);
        m_streamref << std::endl;
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::event(bool &run)
    {
        // Built-in commands
        if (m_command[0] == '.')
        {
            // Try autocompleting any || statements
            if (trueAutoComplete() == false)
                return;
            if (m_command.find(".reg ") == 0)
            {
                interfaceReg();
            }
            else if (m_command == ".list")
            {
                listAll();
            }
            else if (m_command.find(".list ") == 0)
            {
                listCommand();
            }
            else if (m_command.find(".load ") == 0)
            {
                interfaceLoad();
            }
            else if (m_command.find(".store ") == 0)
            {
                interfaceStore();
            }
            else if (m_command.find(".add ") == 0)
            {
                interfaceAdd();
            }
            else if (m_command.find(".erase ") == 0)
            {
                interfaceErase();
            }
            else if (m_command.find(".erase") == 0)
            {
                interfaceErase();
            }
            else if (m_command == ".clear")
            {
                clear(); // Invalidates iterator
                m_last = m_ptr->m_registry.end();
            }
            else if (m_command == ".exit")
            {
                run = false;
            }
            else if (m_command == ".h")
            {
                displayHelp();
            }
            else
            {
                m_streamref << "Unable to interpret command";
            }
        }
        else
        {
            // We need to issue something...
            change();
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::displayHelp()
    {
        m_streamref
            << "Enter \".list\" to list all registers"
            << std::endl
            << "Enter \".list\" + a string to list all registers containing this string (\".list x\")"
            << std::endl
            << "Enter \".reg\" and the register name to complete it (\".reg x\")"
            << std::endl
            << "Enter anything to update the active register (no whitespaces) \"(800,600)\""
            << std::endl
            << "Enter \".clear\" to remove all entries"
            << std::endl
            << "Enter \".add x\" to add entry x"
            << std::endl
            << "Enter \".add x y\" to add entry x with the value of y"
            << std::endl
            << "Enter \".erase\" to erase the active registry"
            << std::endl
            << "Enter \".erase x\" to erase the entry"
            << std::endl
            << "Enter \".load x\" to load file x"
            << std::endl
            << "Enter \".store x\" to store the registry to file x"
            << std::endl
            << "Enter the start of the register name and append || to invoke auto-complete \".command x||\""
            << std::endl
            << "Enter \".exit\" to return";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::listAll()
    {
        bool at_least_one_entry = false;

        for (decltype(m_ptr->m_registry)::iterator it = m_ptr->m_registry.begin(); it != m_ptr->m_registry.end(); ++it)
        {
            if (at_least_one_entry)
            {
                m_streamref << std::endl;
            }

            m_streamref
                << it->first
                << " = "
                << it->second;
            at_least_one_entry = true;
        }

        if (at_least_one_entry == false)
        {
            m_streamref
                << "No entries";
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::listCommand()
    {
        std::string wtv;
        wtv.resize(m_command.size() - 6);
        std::copy(m_command.begin() + 6, m_command.end(), wtv.begin());

        bool at_least_one_entry = false;

        for (auto &x : m_ptr->m_registry)
        {
            if (x.first.find(wtv) != std::string::npos)
            {
                if (at_least_one_entry)
                {
                    m_streamref << std::endl;
                }
                m_streamref
                    << x.first
                    << " = "
                    << x.second;
                at_least_one_entry = true;
            }
        }

        if (at_least_one_entry == false)
        {
            m_streamref << "No entries found that match the criteria";
        }
    }

    ////////////////////////////////////////////////////////////
    // Editor
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    Valman::Editor::Editor(Valman *ptr, std::ostream &streamref)
    :
        m_ptr(ptr),
        m_streamref(streamref),
        m_last(ptr->m_registry.end())
    {}

} // Namespace ttl
