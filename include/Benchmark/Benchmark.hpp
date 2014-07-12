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


#ifndef BENCHMARK_HPP_INCLUDED
#define BENCHMARK_HPP_INCLUDED

// Headers
#include <utility>
#include <chrono>
#include <ostream>
#include <functional>
#include "../include/Ttldef/Ttldef.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief A benchmarking class
    ///
    /// A class designed for handling benchmarks of functions.
    /// It is possible to use member functions, lambdas,
    /// function pointers and const function pointers (normal
    /// functions).
    ///
    ////////////////////////////////////////////////////////////
    class Benchmark
    {
    private:

        typedef std::chrono::nanoseconds ns;
        typedef std::chrono::microseconds us;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds s;
        typedef std::chrono::minutes m;
        typedef std::chrono::hours h;

        typedef std::chrono::high_resolution_clock hre;
        typedef std::chrono::time_point<hre> tphre;

    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param title The title to give this benchmark
        /// \param iterations The amount of calls to benchmark
        ///
        ////////////////////////////////////////////////////////////
        Benchmark(const char *title = "Unnamed benchmark", const Sti_t iterations = 1);

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param title The title to give this benchmark
        /// \param iterations The amount of calls to benchmark
        ///
        ////////////////////////////////////////////////////////////
        explicit Benchmark(const std::string &title, const Sti_t iterations = 1);

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param iterations The amount of calls to benchmark
        ///
        ////////////////////////////////////////////////////////////
        Benchmark(const Sti_t iterations = 1);

        ////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// \param benchmark The benchmark to copy
        ///
        ////////////////////////////////////////////////////////////
        Benchmark(const Benchmark &benchmark);

        ////////////////////////////////////////////////////////////
        /// \brief Move constructor
        ///
        /// \param benchmark The benchmark to move
        ///
        ////////////////////////////////////////////////////////////
        Benchmark(Benchmark &&benchmark);

        ////////////////////////////////////////////////////////////
        /// \brief Copy assignment
        ///
        /// \param benchmark The benchmark to move
        /// \return A reference to itself
        ///
        ////////////////////////////////////////////////////////////
        Benchmark &operator=(const Benchmark &benchmark);

        ////////////////////////////////////////////////////////////
        /// \brief Move assignment
        ///
        /// \param benchmark The benchmark to move
        /// \return A reference to itself
        ///
        ////////////////////////////////////////////////////////////
        Benchmark &operator=(Benchmark &&benchmark);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~Benchmark();

        ////////////////////////////////////////////////////////////
        /// \brief Actual running algorithm
        ///
        /// Will loop over the function n times specified by
        /// setIterations or by the constructor. The running time
        /// will be added to the old running time and then halfed
        /// in order to approximate an average.
        ///
        /// \param args first a function and then optional arguments
        ///
        ////////////////////////////////////////////////////////////
        template </*typem_name Fun, */typename ...Args>
        void run(/*Fun &&fnc, */Args &&...args)
        {
            tphre before, after;
            auto fnc = std::bind(/*fnc, */std::forward<Args>(args)...);
            before = hre::now();

            for (Sti_t i = 0; i < m_iterations; ++i)
    //            fnc(std::forward<Args>(args)...); // Does not work with methods
                fnc(); // Works with methods
            after = hre::now();
            m_average = ns(static_cast<Sti_t>(((std::chrono::duration_cast<ns>(m_average).count() + std::chrono::duration_cast<ns>(after - before).count() / static_cast<float>(m_iterations))) / 2.f));
        }

        ////////////////////////////////////////////////////////////
        /// \brief Reset the average running time
        ///
        ////////////////////////////////////////////////////////////
        void resetAverageRunTime();

        ////////////////////////////////////////////////////////////
        /// \brief Set the amount of iterations per benchmark
        ///
        ////////////////////////////////////////////////////////////
        void setIterations(const Sti_t amount);

        ////////////////////////////////////////////////////////////
        /// \brief Get the average running time per benchmark
        ///
        /// \return the running time in microseconds
        ///
        ////////////////////////////////////////////////////////////
        Benchmark::ns getAverageRunTime() const;

        ////////////////////////////////////////////////////////////
        /// \brief Overload of the ostream operator
        ///
        /// Outputs the name of the benchmark and the time
        /// cost per iteration.
        ///
        /// \param lhs the stream
        /// \param rhs the benchmark object
        /// \return A reference to the std::ostream object
        ///
        ////////////////////////////////////////////////////////////
        friend std::ostream &operator<<(std::ostream &lhs, const Benchmark &rhs);

    private:

        Sti_t m_iterations; ///< Iterations per run
        std::chrono::nanoseconds m_average; ///< Average running time per call
        std::string m_name; ///< Title of this benchmark
    };

} // Namespace ttl

#endif // BENCHMARK_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Benchmark
/// \ingroup Programming Utilities
///
/// Benchmark is used to easily set up a benchmarking
/// environment for any function. The class has a high
/// amount of dynamicity and allows for virtually any
/// function to be called, even virtual functions.
///
///
/// \code
/// // Declare a new benchmarking unit
/// ttl::Benchmark ben("If statement", 10000);
///
/// // Run the benchmark
/// ben.run
/// (
///     // Use this lambda function
///     [](int value)
///     {
///         if (value % 2)
///             value = 0;
///     },
///     0 // With this as argument
/// );
///
/// // Output the results to the std::cout std::ostream
/// std::cout << ben << std::endl;
/// \endcode
///
/// For virtual functions and non-lambda functions;
/// the syntax that goes into run is forwarded into
/// std::bind(...), so the argument order is the same.
///
/// \code
/// // Declare and define a base and derived class
/// class Base{public: virtual void fnc(volatile int &num){} virtual ~Base(){}};
/// class Derived : public Base{public:virtual void fnc(volatile int &num) override{num++;}};
///
/// // Declare a number volatile to avoid compiler optimizations
/// volatile int number = 0;
///
/// // Create an object of the class
/// Base *bptr = new Derived;
///
/// // Declare a benchmarking object, 100k iterations
/// Benchmark virtual_call("Virtual method call", 10000000);
///
/// // Run the benchmark
/// virtual_call.run
/// (
///     &Base::fnc, // use this function
///     bptr, // and this object pointer (this)
///     std::ref(number) // with this argument, must explicitly use std::ref
/// );
///
/// // Delete the pointer
/// delete bptr;
///
/// // Output the data to our console window
/// std::cout << virtual_call;
///
/// // Now we know for sure if the virtual function was delegated or not:
/// std::cout << "number: " << number << std::endl;
/// \endcode
///
////////////////////////////////////////////////////////////
