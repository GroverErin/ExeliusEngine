#pragma once
#include "source/utility/containers/RingBuffer.h"
#include "source/utility/generic/SmartPointers.h"

#include <EASTL/functional.h>
#include <EASTL/vector.h>
#include <condition_variable>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct Job
	{
		SharedPtr<Job> m_pParentJob;
		eastl::function<void()> m_job;
		std::atomic<uint16_t> m_jobCounter;

		Job& operator=(const Job& other)
		{
			m_pParentJob = other.m_pParentJob;
			m_job = other.m_job;
			m_jobCounter = other.m_jobCounter.load();
			return *this;
		}
	};

	class JobSystem
	{
		RingBufferMT<SharedPtr<Job>, 256> m_jobPool;
		std::atomic<uint32_t> m_jobCounter;
		std::condition_variable m_jobSignal;
		std::mutex m_jobLock;
		uint8_t m_threadCount;

	public:
		JobSystem();

		bool Initialize();

		const SharedPtr<Job> PushJob(const eastl::function<void()>& jobToPush, SharedPtr<Job> pParentJob = nullptr);

		bool JobsAreExecuting();

		void WaitForJob(uint64_t jobToWaitFor);

		void WaitForAllJobs();

	private:
		void CycleThread();
		void ExecuteJob();
		void RecurseCounterDecrement(SharedPtr<Job> pJob);
	};

	inline static JobSystem* s_pGlobalJobSystem = nullptr;
}