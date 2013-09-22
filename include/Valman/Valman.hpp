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

    ////////////////////////////////////////////////////////////
    /// \brief Value manager and loader
    ///
    /// Contains a CLI editor that wraps around its core
    /// functionality. The loader is a very simple one,
    /// it parses using whitespaces as separators.
    /// The data is stored in a hash map of strings.
    ///
    ////////////////////////////////////////////////////////////
    class Valman
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructs a basic object
        ///
        ////////////////////////////////////////////////////////////
        Valman();

        ////////////////////////////////////////////////////////////
        /// \brief Constructs a basic object and loads a single file
        ///
        /// \param filename the file to load from.
        ///
        ////////////////////////////////////////////////////////////
        Valman(const std::string &filename);

        ////////////////////////////////////////////////////////////
        /// \brief Destructs the object
        ///
        ////////////////////////////////////////////////////////////
        ~Valman();

        // Overloaded access operators
        ////////////////////////////////////////////////////////////
        /// \brief A range-checked access operator
        ///
        /// This function checks if str exists in the hash table.
        /// If it does not, an std::runtime_error is thrown.
        ///
        ////////////////////////////////////////////////////////////
        std::string &at(const std::string &str);

        ////////////////////////////////////////////////////////////
        /// \brief Access operator
        ///
        /// This access operator does not perform a range check.
        ///
        ////////////////////////////////////////////////////////////
        std::string &operator[](const std::string &str);

        // Utility fncs
        ////////////////////////////////////////////////////////////
        /// \brief Removes all elements
        ///
        ////////////////////////////////////////////////////////////
        void clear();

        ////////////////////////////////////////////////////////////
        /// \brief Loads data from a file
        ///
        /// \param filename is the file to load from
        /// \return the state of loading, false for failure
        ///
        ////////////////////////////////////////////////////////////
        bool load(const std::string &filename);

        ////////////////////////////////////////////////////////////
        /// \brief Add data to the hash table
        ///
        /// \param value is a pair of data, both strings.
        ///
        ////////////////////////////////////////////////////////////
        void add(const std::pair<std::string, std::string> &value);

        ////////////////////////////////////////////////////////////
        /// \brief Add data to the hash table
        ///
        /// This add will leave the value empty ("")
        ///
        /// \param data is the string key to be added
        ///
        ////////////////////////////////////////////////////////////
        void add(const std::string &data);

        ////////////////////////////////////////////////////////////
        /// \brief Erase an entry from the hash table
        ///
        /// \param entry is the key to erase
        ///
        ////////////////////////////////////////////////////////////
        void erase(const std::string &entry);

        ////////////////////////////////////////////////////////////
        /// \brief Stores the hash table into a file
        ///
        /// \param filename the file to store into
        ///
        ////////////////////////////////////////////////////////////
        void store(const std::string &filename);

        ////////////////////////////////////////////////////////////
        /// \brief Look if a key exists
        ///
        /// \param key is the key to look for
        /// \return an iterator to either end or the valid data pair
        ///
        ////////////////////////////////////////////////////////////
        const std::unordered_map<std::string, std::string>::const_iterator find(const std::string &key) const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the iterator to the end
        ///
        /// \return an iterator past the last element
        ///
        ////////////////////////////////////////////////////////////
        const std::unordered_map<std::string, std::string>::const_iterator end() const;

        ////////////////////////////////////////////////////////////
        /// \brief Enter the CLI
        ///
        ////////////////////////////////////////////////////////////
        void edit();

        ////////////////////////////////////////////////////////////
        /// \brief Enter the CLI with a single command
        ///
        ////////////////////////////////////////////////////////////
        void edit(const std::string &command);

        ////////////////////////////////////////////////////////////
        /// \brief Enter the CLI whilst specifying an output stream
        ///
        ////////////////////////////////////////////////////////////
        std::stringstream &edit(const std::string &command, std::stringstream &output);

        ////////////////////////////////////////////////////////////
        /// \brief Enter the CLI whilst specifying an output stream
        ///
        ////////////////////////////////////////////////////////////
        std::ostream &edit(const std::string &command, std::ostream &output);

    private:

        class Editor
        {
        public:

            Editor(Valman *ptr, std::ostream &streamref);

            bool autoComplete(std::string &to_complete);
            bool trueAutoComplete(); // This one takes ANY string, no matter how messy!
            void clear();

            // Interface sub
            void interfaceLoad();
            void interfaceAdd();
            void interfaceErase();
            void interfaceStore();
            void interfaceReg();

            // Core UI
            void change();
            void prompt();
            void event(bool &run);

            // list all entries
            void displayHelp(); // contains, command, input, data, line, arg
            void listAll();
            void listCommand();

            std::string m_command;

        private:

            Valman *m_ptr;
            std::ostream &m_streamref;
            std::unordered_map<std::string, std::string>::iterator m_last;
        };

//        static bool isOperator(const char in);
        static constexpr const char *shortcut = "||";

        std::unordered_map<std::string, std::string> m_registry;
    };

} // Namespace ttl

#endif // VALMAN_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Valman
/// \ingroup Utilities
///
/// This value manager can be used in order to load and store
/// simple data strings. The files are easily readable by a
/// standard text editor.
///
/// \code
/// ttl::Valman val("B");
/// val.add("x");
/// val.edit();
/// std::cout << std::stoi(val["x"]) << std::endl;
/// \endcode
///
////////////////////////////////////////////////////////////
