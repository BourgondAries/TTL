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

    class Rexception : public std::exception
    {
    public:

        Rexception();
        Rexception(Runnable *ptr);
        Rexception(const char *message, Runnable *ptr);
        virtual ~Rexception();
        virtual const char *what() const noexcept;

    private:

        const char *m_message;
        Runnable *m_return_point;

    };

} // Namespace ttl

#endif // RUNNABLE_HPP_INCLUDED
