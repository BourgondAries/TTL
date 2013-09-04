#ifndef VALMAN_HPP_INCLUDED
#define VALMAN_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>


namespace ttl
{
    class Valman
    {
    public:

        // Standard member functions
        Valman();
        Valman(const char *filename);
        ~Valman();

        // Overloaded access operators
        float &operator()(const char *entry);
        float &operator[](const char *entry);

        // Utility fncs
        void clear();
        bool load(const char *filename);
        void add(const std::pair<std::string, float> &value);
        void add(const std::string &data);
        void erase(const std::string &entry);
        void store(const char *filename);
        bool find(const std::string &data);

        void edit();
        void edit(const std::string &command);
        std::stringstream &edit(const std::string &command, std::stringstream &output);
        std::ostream &edit(const std::string &command, std::ostream &output);

    private:

        class Editor
        {
        public:

            Editor(Valman *ptr, std::ostream &streamref);

            bool autoComplete(std::string &to_complete);
            bool trueAutoComplete(); // This one takes ANY string, no matter how messy!
            void sqrt();
            void pow();
            void clear();

            // Interface sub
            void interfaceLoad();
            void interfaceAdd();
            void interfaceErase();
            void interfaceStore();

            // Core UI
            void change();
            void prompt();
            void event(bool &run);

            // list all entries
            void displayHelp() const; // contains, command, input, data, line, arg
            void list();
            void listCommand();

            std::string m_command;

        private:

            Valman *m_ptr;
            std::ostream &m_streamref;
            std::unordered_map<std::string, float>::iterator m_last;
        };

        // Subroutines
        static float extractFirstNumber(std::string &line);
        static void eraseLastAssigns(std::string &line);
        static bool isNumeric(const char in);
        static bool isAssignment(const char in);
        static bool isOperator(const char in);


        static constexpr const char *shortcut = "||";

        std::unordered_map<std::string, float> m_registry;
    };

} // Namespace ttl

#endif // VALMAN_HPP_INCLUDED
