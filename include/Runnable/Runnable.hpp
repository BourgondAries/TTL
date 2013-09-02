#ifndef RUNNABLE_HPP_INCLUDED
#define RUNNABLE_HPP_INCLUDED

#include <memory>

namespace ttl
{

    class Runnable
    {
    public:

        Runnable() = default;
        virtual ~Runnable() = default;

        virtual Runnable *run() = 0;

        static void cycle(std::unique_ptr<Runnable> runnable);
    };

} // Namespace ttl

#endif // RUNNABLE_HPP_INCLUDED
