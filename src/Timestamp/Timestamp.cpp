#include <Timestamp/Timestamp.hpp>

////////////////////////////////////////////////////////////
namespace ttl
{

    std::string getTimeStamp()
    {
        typedef std::chrono::system_clock chrsc;
        typedef std::chrono::time_point<chrsc> chrtp;

        chrtp now = std::chrono::system_clock::now();
        chrtp epoch;

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - epoch).count();

        std::time_t time = std::chrono::system_clock::to_time_t(now);

        duration %= 1000000000;
        std::string str(std::ctime(&time));

        std::string::iterator it = --str.end();
        for (; it != str.begin(); --it)
        {
            if (*it == ' ')
            {
                break;
            }
        }

        // Make sure duration has enough prefix 0:
        std::string duration_str = std::move(std::to_string(duration));
        while (duration_str.size() < 9)
            duration_str.insert(duration_str.begin(), '0');

        std::string dur = "." + duration_str;
        str.insert(it, dur.begin(), dur.end());
        str.erase(--str.end());

        return str;
    }

} // Namespace ttl
