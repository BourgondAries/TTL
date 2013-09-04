////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <fstream>
#include <sstream>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    std::string file2str(const std::string &filename)
    {
        std::fstream input_file(filename.data(), std::ios::in);
        std::stringstream ss;
        ss << input_file.rdbuf();
        return ss.str();
    }

} // Namespace ttl
