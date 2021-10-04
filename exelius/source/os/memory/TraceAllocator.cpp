#include "EXEPCH.h"
#include "TraceAllocator.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	TraceAllocator::TraceAllocator()
		: m_pParentAllocator(nullptr)
		, m_pMemoryLog(nullptr)
		, m_allocationCount(0)
		, m_totalAllocatedBytes(0)
	{
		//
	}

	TraceAllocator::TraceAllocator(ExeliusAllocator* pParentAllocator)
		: m_pParentAllocator(pParentAllocator)
		, m_pMemoryLog(nullptr)
		, m_allocationCount(0)
		, m_totalAllocatedBytes(0)
	{
		EXE_ASSERT(m_pParentAllocator);
	}

	TraceAllocator::~TraceAllocator()
	{
		EXELIUS_DELETE(m_pMemoryLog);
	}

	void TraceAllocator::SetParentAllocator(ExeliusAllocator* pParentAllocator)
	{
		m_pParentAllocator = pParentAllocator;
	}
	
	void* TraceAllocator::Allocate(size_t sizeToAllocate, const char* pFileName, int lineNum)
	{
		++m_allocationCount;
		m_totalAllocatedBytes += sizeToAllocate;

		EXE_ASSERT(m_pParentAllocator);
		return m_pParentAllocator->Allocate(sizeToAllocate, pFileName, lineNum);
	}

	void TraceAllocator::Free(void* memoryToFree, size_t sizeToFree)
	{
		EXE_ASSERT(--m_allocationCount >= 0);
		m_totalAllocatedBytes -= sizeToFree;
		EXE_ASSERT(m_totalAllocatedBytes >= 0);

		EXE_ASSERT(m_pParentAllocator);
		return m_pParentAllocator->Free(memoryToFree);
	}

	void TraceAllocator::DumpMemoryData()
	{
		if (!m_pMemoryLog)
			m_pMemoryLog = EXELIUS_NEW(Log("MemoryManager"));

		EXE_ASSERT(m_pMemoryLog);
		m_pMemoryLog->Info("Current Total Allocation Count: {}", m_allocationCount);
		m_pMemoryLog->Info("Current Total Memory Allocated: {}", m_totalAllocatedBytes);
	}
}