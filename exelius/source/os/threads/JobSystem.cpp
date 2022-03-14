#include "EXEPCH.h"
#include "JobSystem.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    JobSystem::JobSystem()
        : m_jobCounter(0)
        , m_threadCount(0)
    {
        //
    }

    bool JobSystem::Initialize()
    {
        m_threadCount = static_cast<uint8_t>(std::thread::hardware_concurrency() - 1);

        for (uint8_t threadID = 0; threadID < m_threadCount; ++threadID)
        {
            std::thread worker(&JobSystem::ExecuteJob, this);

            // Not super ideal, might be better to create
            // and size a fixed sized array of threads.
            worker.detach();
        }

        return true;
    }

    const SharedPtr<Job> JobSystem::PushJob(const eastl::function<void()>& jobToPush, SharedPtr<Job> pParentJob /* = nullptr */)
    {
        SharedPtr<Job> pNewJob = MakeShared<Job>();
        pNewJob->m_pParentJob = pParentJob;
        ++pNewJob->m_jobCounter;
        pNewJob->m_job = jobToPush;
        if (pParentJob)
            ++pParentJob->m_jobCounter;
        ++m_jobCounter;

        while (!m_jobPool.PushBack(pNewJob))
        {
            CycleThread();
        }

        m_jobSignal.notify_one();

        return pNewJob;
    }

    bool JobSystem::JobsAreExecuting()
    {
        return (m_jobCounter != 0);
    }

    void JobSystem::WaitForJob(uint64_t jobToWaitFor)
    {
        while (jobToWaitFor != m_jobCounter)
        {
            CycleThread();
        }
    }

    void JobSystem::WaitForAllJobs()
    {
        while (JobsAreExecuting())
        {
            CycleThread();
        }
    }

    void JobSystem::CycleThread()
    {
        m_jobSignal.notify_one();
        std::this_thread::yield();
    }

    void JobSystem::ExecuteJob()
    {
        SharedPtr<Job> pJobToExecute;

        while (true)
        {
            if (m_jobPool.PopFront(pJobToExecute))
            {
                pJobToExecute->m_job();
                RecurseCounterDecrement(pJobToExecute);
            }
            else
            {
                std::unique_lock<std::mutex> lock(m_jobLock);
                m_jobSignal.wait(lock);
            }
        }
    }

    void JobSystem::RecurseCounterDecrement(SharedPtr<Job> pJob)
    {
        --pJob->m_jobCounter;
        if (pJob->m_pParentJob)
            RecurseCounterDecrement(pJob->m_pParentJob);
        --m_jobCounter;
    }
}
