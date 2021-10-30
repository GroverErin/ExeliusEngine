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

    const Job& JobSystem::PushJob(const eastl::function<void()>& jobToPush, Job* pParentJob /* = nullptr */)
    {
        Job newJob;
        newJob.m_pParentJob = pParentJob;
        ++newJob.m_jobCounter;
        newJob.m_job = jobToPush;
        if (pParentJob)
            ++pParentJob->m_jobCounter;
        ++m_jobCounter;

        while (!m_jobPool.PushBack(newJob))
        {
            CycleThread();
        }

        m_jobSignal.notify_one();

        return newJob;
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
        Job jobToExecute;

        while (true)
        {
            if (m_jobPool.PopFront(jobToExecute))
            {
                jobToExecute.m_job();
                RecurseCounterDecrement(&jobToExecute);
            }
            else
            {
                std::unique_lock<std::mutex> lock(m_jobLock);
                m_jobSignal.wait(lock);
            }
        }
    }

    void JobSystem::RecurseCounterDecrement(Job* pJob)
    {
        --pJob->m_jobCounter;
        if (pJob->m_pParentJob)
            RecurseCounterDecrement(pJob->m_pParentJob);
        --m_jobCounter;
    }
}
