////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ScopedFunction/ScopedFunction.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction()
    :
        fn([](){})
    {}

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction(const ScopedFunction &scopedfunction)
    :
        fn(scopedfunction.fn)
    {
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction::ScopedFunction(ScopedFunction &&scopedfunction)
    :
        fn(scopedfunction.fn)
    {
        scopedfunction.fn = [](){};
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction &ScopedFunction::operator=(const ScopedFunction &scopedfunction)
    {
        fn = scopedfunction.fn;
        return *this;
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction &ScopedFunction::operator=(ScopedFunction &&scopedfunction)
    {
        fn = scopedfunction.fn;
        scopedfunction.fn = [](){};
        return *this;
    }

    ////////////////////////////////////////////////////////////
    void ScopedFunction::reset()
    {
        fn = [](){};
    }

    ////////////////////////////////////////////////////////////
    ScopedFunction::~ScopedFunction()
    {
        fn();
    }

} // Namespace ttl
