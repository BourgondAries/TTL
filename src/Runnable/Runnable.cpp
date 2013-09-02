#include "Runnable/Runnable.hpp"
#include "Logger/Logger.hpp"


namespace ttl
{

    void Runnable::cycle(std::unique_ptr<Runnable> runnable)
    {
        try
        {
            Logger<true> system_log("system.log", true, std::ios::trunc | std::ios::out);
            try
            {
                if (runnable)
                {
                    std::size_t cycle_count = 0;
                    Runnable *holder;
                    system_log << Timestamp << "Created temporary\n";

                    do
                    {
                        system_log << Timestamp << "Entering cycle " << ++cycle_count << "\n";
                        system_log << Timestamp << "Pointer valid, calling run()\n";
                        holder = runnable->run();
                        system_log << Timestamp << "Returned from run()\n";
                        if (holder == runnable.get())
                        {
                            system_log << Timestamp << "this returned, recalling run()\n";
                            continue;
                        }
                        runnable.reset(holder);
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
            catch (std::exception &e)
            {
                system_log << Timestamp << "An object of std::exception was caught:\n\twhat(): " << e.what() << "\n";
            }
            catch (...)
            {
                system_log << Timestamp << "An unknown exception was caught\n";
            }
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

} // Namespace ttl
