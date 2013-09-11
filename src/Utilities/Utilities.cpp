#include "Utilities/Utilities.hpp"

namespace ttl
{
    float sif(Synched<Valman> &arg, const std::string &key)
    {
        auto x = arg.getReadAccess();
        std::unordered_map<std::string, std::string>::const_iterator derp = x->find(key);
        return std::stof(derp->second);
    }
}
