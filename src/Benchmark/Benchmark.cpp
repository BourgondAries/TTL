/*
Copyright 2013, 2014 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/


// Headers
#include "Benchmark/Benchmark.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const char *title, const Sti_t iterations)
    :
        m_iterations(iterations),
        m_average(0),
        m_name(title){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const std::string &title, const Sti_t iterations)
    :
        m_iterations(iterations),
        m_average(0),
        m_name(title){}

    ////////////////////////////////////////////////////////////
    Benchmark::Benchmark(const Sti_t iterations)
    :
        m_iterations(iterations),
        m_average(0),
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
        benchmark.m_average = ns();
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
        benchmark.m_average = ns();
        benchmark.m_name = "Unnamed Benchmark";

        return std::ref(*this);
    }

    ////////////////////////////////////////////////////////////
    Benchmark::~Benchmark(){}

    ////////////////////////////////////////////////////////////
    void Benchmark::resetAverageRunTime()
    {
        m_average = ns();
    }

    ////////////////////////////////////////////////////////////
    void Benchmark::setIterations(const Sti_t amount)
    {
        m_iterations = amount;
    }

    ////////////////////////////////////////////////////////////
    Benchmark::ns Benchmark::getAverageRunTime() const
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

        if (rhs.m_average > h(1))
            lhs << rhs.m_average.count() / (1E9f * 3600.f) << " h" << std::endl;
        else if (rhs.m_average > m(1))
            lhs << rhs.m_average.count() / (1E9f * 60.f) << " min" << std::endl;
        else if (rhs.m_average > s(1))
            lhs << rhs.m_average.count() / 1E9f << " s" << std::endl;
        else if (rhs.m_average > ms(1))
            lhs << rhs.m_average.count() / 1E6f << " ms" << std::endl;
        else if (rhs.m_average > us(1))
            lhs << rhs.m_average.count() / 1E3f << " µs" << std::endl;
        else
            lhs << rhs.m_average.count() << " ns" << std::endl;

        return lhs;
    }

} // Namespace ttl
