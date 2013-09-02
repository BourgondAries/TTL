////////////////////////////////////////////////////////////
//
// TTL - Tea Tank Library
// Copyright (C) 2013 Kevin R. Stravers (macocio@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
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
    Valman::Valman(const char *filename)
    {
        load(filename);
    }

    ////////////////////////////////////////////////////////////
    Valman::~Valman()
    {
    }

    ////////////////////////////////////////////////////////////
    // Overloaded access operators
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    float &Valman::operator()(const char *entry)
    {
        // Check the validity of the m_commandument
        std::string temp(entry);
        std::unordered_map<std::string, float>::iterator it = m_registry.find(temp);
        if (it == m_registry.end())
            throw std::invalid_argument("Valman::operator()(\"" + temp + "\")");
        return it->second;
    }

    ////////////////////////////////////////////////////////////
    float &Valman::operator[](const char *entry)
    {
        return m_registry.find(std::string(entry))->second;
    }

    ////////////////////////////////////////////////////////////
    // Utility functions
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    void Valman::clear()
    {
        if (m_registry.size() > 0)
        {
            m_registry.clear();
            std::cout << "All entries cleared";
        }
        else
        {
            std::cout << "Registry is already empty";
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::load(const char *filename)
    {
        // Generic loader from file:
        std::fstream file(filename, std::ios::in);

        // Use this as a m_command buffer
        std::string buffer;
        buffer.push_back(file.get());

        // Make sure the file exists
        if (!file.is_open())
        {
            return false;
        }

        // Iterate over every char in the file
        while (!file.eof())
        {
            // So long there is no \n, add it to the buffer:
            while (buffer.back() != '\n' && !file.eof())
            {
                buffer.push_back(file.get());
            }

            if // Check if the m_command doesn't m_m_command a number AND assigner
            (
                std::any_of
                (
                    buffer.begin(), buffer.end(),
                    [this](const char &x) -> bool
                    {
                        return this->isNumeric(x);
                    }
                ) == false
                ||
                std::any_of
                (
                    buffer.begin(), buffer.end(),
                    [this](const char &x) -> bool
                    {
                        return this->isAssignment(x);
                    }
                ) == false
            )
            {
                buffer.clear();
                buffer.push_back(file.get());
                continue;
            }

            // We now have a single, valid m_command, and need to extract the m_command
            float number = extractFirstNumber(buffer); // modifies the buffer
            eraseLastAssigns(buffer); // modifies the buffer

            // Add it to the registry
            m_registry.insert(std::pair<std::string, float>(buffer, number));

            // Get ready for a restart
            buffer.clear();
            buffer.push_back(file.get());
        }
        return true; // Notify the caller that reading succeeded
    }

    ////////////////////////////////////////////////////////////
    void Valman::add(const std::pair<std::string, float> &value)
    {
        m_registry.insert(value);
    }

    ////////////////////////////////////////////////////////////
    void Valman::add(const std::string &m_command)
    {
        std::string copy(m_command);
        float val = extractFirstNumber(copy);
        eraseLastAssigns(copy);

        m_registry.insert(std::pair<std::string, float>(copy, val));
    }

    ////////////////////////////////////////////////////////////
    void Valman::erase(const std::string &entry)
    {
        m_registry.erase(entry);
    }

    ////////////////////////////////////////////////////////////
    void Valman::store(const char *filename)
    {
        std::fstream file(filename, std::ios::out | std::ios::trunc);

        for (auto &x : m_registry)
        {
            file << x.first;
            file << "=";
            file << x.second;
            file << std::endl;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::find(const std::string &m_command)
    {
        return m_registry.find(m_command) != m_registry.end();
    }

    ////////////////////////////////////////////////////////////
    void Valman::edit()
    {
        std::cout
            << "------------------ Valman v0.5 ------------------"
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
    float Valman::extractFirstNumber(std::string &m_command)
    {
        std::stringstream converter;
        bool hioac = false; // has iterated over assignment char, space, =, : to start looking for numbers only after these. This allows names with numbers in them

        // Find out where the first number begins
        auto location = std::find_if
        (
            m_command.begin(), m_command.end(),
            [&hioac](const char &check) -> bool
            {
                if (hioac)
                {
                    return isNumeric(check);
                }
                else
                {
                    if (isAssignment(check))
                    {
                        hioac = true;
                    }
                    return false;
                }
            }
        );

        // Create a new string and store the number
        std::string number;
        number.resize(std::distance(location, m_command.end()));
        std::copy(location, m_command.end(), number.begin());

        // Remove the number from the string
        m_command.erase(location, m_command.end());

        // Convert to a float and return
        converter << number;
        float value = 0.f;
        converter >> value;
        return value;
    }

    ////////////////////////////////////////////////////////////
    void Valman::eraseLastAssigns(std::string &m_command)
    {
        // Find the first non-assignment and delete everything after it. in: derp = , out: derp
        for (auto it = m_command.rbegin(); it != m_command.rend(); ++it)
        {
            if (isAssignment(*it) == false)
            {
                m_command.erase(std::distance(it, m_command.rend()), std::distance(m_command.begin(), m_command.end()));
                break;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::isNumeric(const char in)
    {
        switch (in)
        {
            case '+':
            case '-':
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
            default:
                return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::isAssignment(const char in)
    {
        switch (in)
        {
            case '=':
            case ':':
            case ' ':
                return true;
            default:
                return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::isOperator(const char in)
    {
        switch (in)
        {
            case '*':
            case '+':
            case '-':
            case '/':
            case '%':
                return true;
            default:
                return false;
        }
    }

    ////////////////////////////////////////////////////////////
    bool Valman::Editor::autoComplete(std::string &to_complete) // Accepts a string and tries to match it
    {
    //    m_streamref << "autoComplete(" << m_command << ")" << std::endl;
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
        // We need a truly generic expansion system.
        /*
            Possible m_commands: (worst case, most chaos)
            ./whasadsajk dsajdsakdsaj dsajkjlkad|| dsajdsa sad 432
        */

        // First, find if || occurs, if it does not, return true (We're done)
        // return false is for /autoCompletes that can not be interpreted(too many entries, for example)/
        while (m_command.find(shortcut) != std::string::npos)
        {
            // Create a temporary with only this object:
            std::string pure;
            std::string::iterator insert_location(m_command.begin());

            for (signed int i(static_cast<signed int>(std::distance(m_command.begin(), m_command.begin() + m_command.find_first_of(shortcut)))); i >= 0; --i)
            {
                if (m_command[i] == ' ' || isOperator(m_command[i])) // If there's a space, or a mathematical operator
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

    //    m_streamref << "Result: " << m_command << std::endl;

        return true;
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::sqrt()
    {
        if (m_last != m_ptr->m_registry.end())
        {
            m_last->second = std::sqrt(m_last->second);
            m_streamref << "Active variable: \"" + m_last->first + "\" = " << m_last->second;
        }
        else
        {
            m_streamref << "No active variable";
        }
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::pow()
    {
        m_command.erase(m_command.begin(), m_command.begin() + 4);
        float num(extractFirstNumber(m_command));

        // Remove all numerals and assignment chars
        m_command.erase
        (
            std::remove_if
            (
                m_command.begin(), m_command.end(),
                [](const char &x) ->  bool
                {
                    return isNumeric(x) || isAssignment(x);
                }
            ),
            m_command.end()
        );

        if (m_command.size() > 0) // We have a name!
        {
            decltype(m_ptr->m_registry)::iterator with = m_ptr->m_registry.find(m_command);
            if (m_last != m_ptr->m_registry.end())
            {
                if (with != m_ptr->m_registry.end())
                {
                    m_streamref << "Exponentiation by \"" << with->first << "\" (" << with->second << ")\n";
                    m_last->second = std::pow(m_last->second, with->second);
                    m_streamref << "Active variable: " << m_last->first << " = " << m_last->second;
                }
                else
                {
                    m_streamref << "Entry \"" << m_command << "\" could not be found";
                }
            }
            else
            {
                m_streamref << "Invalid active entry";
            }
        }
        // Now there's only a number...
        else if (m_last != m_ptr->m_registry.end()) // Check if the iterator is valid
        {
            m_streamref << "Exponentiation by (" << num << ")\n";
            m_last->second = std::pow(m_last->second, num);
            m_streamref << "Active variable: " << m_last->first << " = " << m_last->second;
        }
        else
        {
            m_streamref << "Invalid active entry";
        }

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

        if (m_ptr->load(file.data()))
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
        m_command.erase(m_command.begin(), m_command.begin() + 5);

        float val = extractFirstNumber(m_command);
        eraseLastAssigns(m_command);

        m_ptr->m_registry.insert(std::pair<std::string, float>(m_command, val));

        m_streamref
            << "The data is added";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::interfaceErase()
    {
        m_command.erase(m_command.begin(), m_command.begin() + 7);

        extractFirstNumber(m_command);
        eraseLastAssigns(m_command);

        if (m_command.size() == 0) // If no m_command is given, delete active
        {
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
        m_command.erase(m_command.begin(), m_command.begin() + 7);

        m_ptr->store(m_command.data());

        m_streamref
            << "The data is stored";
    }

    ////////////////////////////////////////////////////////////
    // Core UI
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    void Valman::Editor::change()
    {
        // What if we have mathematical operations?
        char math = '\0';
        if (isOperator(m_command[0]))
        {
            math = m_command[0];
            m_command.erase(m_command.begin());

            // We now may have a string like: reg, 32, num~1
            // Check if the only iterms are numeric
            if
            (
                std::all_of
                (
                    m_command.begin(), m_command.end(),
                    [](const char &x) -> bool
                    {
                        return isNumeric(x);
                    }
                )
            )
            {
                m_command.insert(m_command.begin(), '=');
                float val = extractFirstNumber(m_command);
                if (m_last != m_ptr->m_registry.end())
                {
                    if(math == '*')
                    {
                        m_last->second *= val;
                    }
                    else if(math == '/')
                    {
                        m_last->second /= val;
                    }
                    else if(math == '+')
                    {
                        m_last->second += val;
                    }
                    else if(math == '-')
                    {
                        m_last->second -= val;
                    }
                    else if(math == '%')
                    {
                        m_last->second = static_cast<float>(static_cast<long int>(m_last->second) % static_cast<long int>(val));
                    }
                    m_streamref
                        << m_last->first
                        << ": " << m_last->second;
                }
                else
                {
                    m_streamref
                        << "No active variable";
                }
            }
            else if // What if we ONLY have a m_command issued?
            (
                std::none_of
                (
                    m_command.begin(), m_command.end(),
                    [](const char &x) -> bool
                    {
                        return isNumeric(x);
                    }
                )
            )
            {
                if (m_last == m_ptr->m_registry.end())
                {
                    m_streamref << "No active variable";
                    return;
                }

                auto val = m_ptr->m_registry.find(m_command);

                if (val == m_ptr->m_registry.end())
                {
                    m_streamref << "Unable to locate variable: \"" << m_command << "\"";
                    return;
                }

                if(math == '*')
                {
                    m_streamref << "Multiplication by \"" << m_command << "\" (" << val->second << ")";
                    m_last->second *= val->second;
                }
                else if(math == '/')
                {
                    m_streamref << "Division by \"" << m_command << "\" (" << val->second << ")";
                    m_last->second /= val->second;
                }
                else if(math == '+')
                {
                    m_streamref << "Addition by \"" << m_command << "\" (" << val->second << ")";
                    m_last->second += val->second;
                }
                else if(math == '-')
                {
                    m_streamref << "Subtraction by \"" << m_command << "\" (" << val->second << ")";
                    m_last->second -= val->second;
                }
                else if(math == '%')
                {
                    m_streamref << "Modulus by \"" << m_command << "\" (" << val->second << ")";
                    m_last->second = static_cast<float>(static_cast<long int>(m_last->second) % static_cast<long int>(val->second));
                }

                m_streamref
                    << std::endl
                    << "Active variable: \"" + m_last->first + "\" = " + std::to_string(m_last->second);
            }
            else
            {
                m_streamref << "Unable to interpret data";
            }
        }

        // If the user issued a pure number, and nothing else
        // Just edit the m_last iterator, if it's valid
        else if
        (
            std::all_of
            (
                m_command.begin(), m_command.end(),
                [](const char &x) -> bool
                {
                    return isNumeric(x);
                }
            )
        )
        {
            m_command.insert(m_command.begin(), '=');
            float val = extractFirstNumber(m_command);
            if (m_last != m_ptr->m_registry.end())
            {
                m_last->second = val;
                m_streamref
                    << m_last->first
                    << ": " << m_last->second;

            }
            else
            {
                m_streamref
                    << "No active variable";
            }
        }
        else if // What if we ONLY have a m_command issued?
        (
            std::none_of
            (
                m_command.begin(), m_command.end(),
                [](const char &x) -> bool
                {
                    return isNumeric(x);
                }
            )
        )
        {
            eraseLastAssigns(m_command);

            m_last = m_ptr->m_registry.find(m_command);

            if (m_last != m_ptr->m_registry.end())
            {
                m_streamref
                    << "Active variable: \"" + m_command + "\" = " << m_ptr->m_registry[m_command];
            }
            else
            {
                m_streamref
                    << "Unable to locate variable";
            }
        }
        else // Both m_command and value
        {
            float val = extractFirstNumber(m_command);
            eraseLastAssigns(m_command);

            m_last = m_ptr->m_registry.find(m_command);

            if (m_last != m_ptr->m_registry.end())
            {
                m_streamref
                    << m_last->first
                    << ": " << val;
                m_last->second = val;
            }
            else
            {
                m_streamref
                    << "Unable to locate variable";
            }
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
        // Try autocompleting any || statements
        if (trueAutoComplete() == false)
        {
            return;
        }

        // Builtin m_commands
        if (m_command[0] == '.')
        {
            if (m_command == ".list" && m_command.size() < 6)
            {
                list();
            }
            else if (m_command.find(".list ") == 0)
            {
                listCommand();
            }
            else if (m_command.find(".sqrt") == 0)
            {
                sqrt();
            }
            else if (m_command.find(".pow ") == 0)
            {
                pow();
            }
            else if (m_command.find(".pow") == 0)
            {
                m_streamref << "Unspecified power value";
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
                m_command += " ";
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
    void Valman::Editor::displayHelp() const
    {
        m_streamref
            << "Enter \".list\" to list all registers"
            << std::endl
            << "Enter \".list\" + an command to list all registers containing this command (\".list x\")"
            << std::endl
            << "Enter the register name to activate it (\"number\")"
            << std::endl
            << "Enter the start of the register name and append || to expand it (\"nu||\")"
            << std::endl
            << "Enter a number to update the active register (\"-.5\")"
            << std::endl
            << "Enter an arithmetic operator and a number to modify the active register (\"*-.5\" or \"+32\")"
            << std::endl
            << "Enter an arithmetic operator and a register name to perform arithmetic (\"*number\" or \"+x||\")"
            << std::endl
            << "Enter a register name and number simultaneously to edit it immediately (\"number -.5\")"
            << std::endl
            << "Enter \".clear\" to remove all entries"
            << std::endl
            << "Enter \".add x\" to add entry x"
            << std::endl
            << "Enter \".add x 3\" to add entry x with the value of 3"
            << std::endl
            << "Enter \".pow x\" to exponentiate the active register by entry x"
            << std::endl
            << "Enter \".pow 2\" to exponentiate the active register by 2"
            << std::endl
            << "Enter \".sqrt\" to take the square root of the active register"
            << std::endl
            << "Enter \".erase x\" to erase the entry"
            << std::endl
            << "Enter \".erase x||\" to erase the entry starting with x"
            << std::endl
            << "Enter \".load x\" to load file x"
            << std::endl
            << "Enter \".store x\" to store the registry to file x"
            << std::endl
            << "Enter \".exit\" to return";
    }

    ////////////////////////////////////////////////////////////
    void Valman::Editor::list()
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







