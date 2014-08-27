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


#ifndef RUNNABLE_HPP_INCLUDED
#define RUNNABLE_HPP_INCLUDED

// Headers
#include <memory>
#include <TTL/Logger/Logger.hpp>
#include <TTL/Ttldef/Ttldef.hpp>


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Class for managing cycles of runnable objects
    ///
    /// Inheriting from this class and overriding its run function
    /// will allow you to send such class into ttl::Runnable::cycle
    ///
    ////////////////////////////////////////////////////////////
    class Runnable
    {
    public:

        Runnable() = default;
        virtual ~Runnable() = default;

        virtual std::unique_ptr<Runnable> run() = 0;

        ////////////////////////////////////////////////////////////
        /// \brief Handles exceptions and logs times.
        ///
        ////////////////////////////////////////////////////////////
        template <bool Log>
        static void cycle(std::unique_ptr<Runnable> runnable)
        {
            try
            {
                ttl::Logger<Log> system_log("system.log", true, std::ios::trunc | std::ios::out);

                do
                {
                    try
                    {
                        if (runnable)
                        {
                            Sti_t cycle_count = 0;
                            std::unique_ptr<Runnable> holder;
                            system_log << Timestamp << "Created temporary\n";

                            do
                            {
                                if (cycle_count == std::numeric_limits<Sti_t>::max())
                                {
                                    cycle_count = 0;
                                    system_log << Timestamp << "Cycle count resetted\n";
                                }
                                system_log << Timestamp << "Entering cycle " << ++cycle_count << "\n";
                                system_log << Timestamp << "Pointer valid, calling run()\n";
                                holder = runnable->run();
                                system_log << Timestamp << "Returned from run()\n";
                                if (holder.get() == runnable.get())
                                {
                                    holder.release();
                                    system_log << Timestamp << "this returned, recalling run()\n";
                                    continue;
                                }
                                runnable = std::move(holder);
                                system_log << Timestamp << "Resetted, checking validity\n";
                            }
                            while (runnable);
                            system_log << Timestamp << "Pointer invalidated, returning\n";
                        }
                        else
                        {
                            system_log << Timestamp << "Pointer invalid, returning\n";
                        }
                    }
                    catch (Runnable *r)
                    {
                        system_log << Timestamp << "An object of ttl::Runnable was caught\n";
                        runnable.reset(r);
                    }
                    catch (std::exception &e)
                    {
                        system_log << Timestamp << "An object of std::exception was caught:\n\twhat(): " << e.what() << "\n";
                        runnable.reset(nullptr);
                    }
                    catch (...)
                    {
                        system_log << Timestamp << "An unknown exception was caught\n";
                        runnable.reset(nullptr);
                    }
                } while (runnable);
            }
            catch (std::exception &e)
            {
                std::cerr << getTimeStamp() << "The logger failed to initialize\n\twhat(): " << e.what() << "\n";
            }
            catch (...)
            {
                std::cerr << getTimeStamp() << "The logger failed to initialize due to an unknown exception\n";
            }
        }
    };

} // Namespace ttl

#endif // RUNNABLE_HPP_INCLUDED
