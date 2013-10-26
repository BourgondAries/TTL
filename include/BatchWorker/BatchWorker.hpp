#ifndef BATCHWORKER_HPP_INCLUDED
#define BATCHWORKER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <thread>
#include <cstddef>
#include <atomic>
#include <vector>
#include "Flare/Flare.hpp"
#include "Worker/Worker.hpp"


namespace ttl
{

    ////////////////////////////////////////////////////////////
    /// \brief Multiple workers that process a batch
    ///
    ////////////////////////////////////////////////////////////
    class BatchWorker
    {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        ////////////////////////////////////////////////////////////
        BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~BatchWorker();

        ////////////////////////////////////////////////////////////
        /// \brief Set the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        void setWorkerCount(const std::size_t workers);

        ////////////////////////////////////////////////////////////
        /// \brief Get the amount of workers
        ///
        ////////////////////////////////////////////////////////////
        std::size_t getWorkerCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Send work to a specified worker
        ///
        /// Work must be within the range [0, getWorkerCount() - 1]
        /// No checks are performed to assure this, it is up to you.
        ///
        ////////////////////////////////////////////////////////////
        template <typename T>
        void issueWork(T t, const std::size_t thread)
        {
            ++m_actively_working;
            m_thread_pool[thread]->issueWork
            (
                [t, this]()
                {
                    t();
                    if (m_actively_working.fetch_sub(1) == 1)
                        m_threads_done.notify();
                }
            );
        }

        ////////////////////////////////////////////////////////////
        /// \brief Wait for all workers to finish their current task.
        ///
        ////////////////////////////////////////////////////////////
        void wait();

    private:

        std::vector<std::unique_ptr<Worker>> m_thread_pool; ///< Collection of workers
        std::atomic<std::size_t> m_actively_working; ///< Counter of actively working workers
        ttl::Flare m_threads_done; ///< Notified when all threads have finished.
    };

} // Namespace ttl

#endif // BATCHWORKER_HPP_INCLUDED
