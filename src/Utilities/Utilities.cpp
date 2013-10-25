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
