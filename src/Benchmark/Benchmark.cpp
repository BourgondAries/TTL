////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Benchmark/Benchmark.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const char *title, const std::size_t iterations)
    :
        m_iterations(iterations),
        m_average(0.f),
        m_name(title){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const std::string &title, const std::size_t iterations)
    :
        m_iterations(iterations),
        m_average(0.f),
        m_name(title){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const std::size_t iterations)
    :
        m_iterations(iterations),
        m_average(0.f),
        m_name("Unnamed Benchmark"){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const Benchmark &benchmark)
    :
        m_iterations(benchmark.m_iterations),
        m_average(benchmark.m_average),
        m_name(benchmark.m_name){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(Benchmark &&benchmark)
    :
        m_iterations(benchmark.m_iterations),
        m_average(benchmark.m_average),
        m_name(benchmark.m_name)
    {
        benchmark.m_iterations = 1;
        benchmark.m_average = 0.f;
        benchmark.m_name = "Unnamed Benchmark";
    }

    ////////////////////////////////////////////////////////////
    Benchmark &Benchmark::operator=(const Benchmark &benchmark)
    {
        m_iterations = benchmark.m_iterations;
        m_average = benchmark.m_average;
        m_name = benchmark.m_name;

        return std::ref(*this);
    }

    ////////////////////////////////////////////////////////////
    Benchmark &Benchmark::operator=(Benchmark &&benchmark)
    {
        m_iterations = benchmark.m_iterations;
        m_average = benchmark.m_average;
        m_name = benchmark.m_name;

        benchmark.m_iterations = 1;
        benchmark.m_average = 0.f;
        benchmark.m_name = "Unnamed Benchmark";

        return std::ref(*this);
    }

    ////////////////////////////////////////////////////////////
    Benchmark::~Benchmark(){}

    ////////////////////////////////////////////////////////////
    void Benchmark::resetAverageRunTime()
    {
        m_average = 0.f;
    }

    ////////////////////////////////////////////////////////////
    void Benchmark::setIterations(const std::size_t amount)
    {
        m_iterations = amount;
    }

    ////////////////////////////////////////////////////////////
    float Benchmark::getAverageRunTime() const
    {
        return m_average;
    }

    ////////////////////////////////////////////////////////////
    std::ostream &operator<<(std::ostream &lhs, const Benchmark &rhs)
    {
        typedef std::chrono::microseconds us;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds s;
        typedef std::chrono::minutes m;
        typedef std::chrono::hours h;

        lhs
            << rhs.m_name << ":" << std::endl
            << "\tT = ";

        if (rhs.m_average > std::chrono::duration_cast<us>(h(1)).count())
            lhs << rhs.m_average / (1E6f * 3600.f) << " h" << std::endl;
        else if (rhs.m_average > std::chrono::duration_cast<us>(m(1)).count())
            lhs << rhs.m_average / (1E6f * 60.f) << " min" << std::endl;
        else if (rhs.m_average > std::chrono::duration_cast<us>(s(1)).count())
            lhs << rhs.m_average / 1E6f << " s" << std::endl;
        else if (rhs.m_average > std::chrono::duration_cast<us>(ms(1)).count())
            lhs << rhs.m_average / 1E3f << " ms" << std::endl;
        else
            lhs << rhs.m_average << " µs" << std::endl;

        return lhs;
    }

} // Namespace tty
