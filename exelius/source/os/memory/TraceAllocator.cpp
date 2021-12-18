#include "EXEPCH.h"
#include "TraceAllocator.h"

#include <iostream>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	TraceAllocator::TraceAllocator()
		: m_pParentAllocator(nullptr)
		, m_allocationCount(0)
		, m_totalAllocatedBytes(0)
		, m_deallocationCount(0)
		, m_totalDeallocatedBytes(0)
	{
		//
	}

	TraceAllocator::TraceAllocator(ExeliusAllocator* pParentAllocator)
		: m_pParentAllocator(pParentAllocator)
		, m_allocationCount(0)
		, m_totalAllocatedBytes(0)
		, m_deallocationCount(0)
		, m_totalDeallocatedBytes(0)
	{
		EXE_ASSERT(m_pParentAllocator);
	}

	void TraceAllocator::SetParentAllocator(ExeliusAllocator* pParentAllocator)
	{
		m_pParentAllocator = pParentAllocator;
	}
	
	void* TraceAllocator::Allocate(size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum)
	{
		if (!pFileName)
			pFileName = "null";

		EXE_ASSERT(m_pParentAllocator);
		void* allocatedMemory = m_pParentAllocator->Allocate(sizeToAllocate, memoryAlignment, pFileName, lineNum);
		EXE_ASSERT(allocatedMemory);

		++m_allocationCount;
		m_totalAllocatedBytes += sizeToAllocate;

		// O(n) loop through array. I could take the BleachLeakDetector route and use a
		// hashmap and hash the line+filename, but that would also allocate memory in some cases.
		// This route will never allocate memory while this tracer is in effect.
		for (auto& entry : m_trackedMemory)
		{
			// If memory in this entry is nullptr, fill the entry.
			if (entry.memoryAddress == 0)
			{
				entry.memoryAddress = reinterpret_cast<uintptr_t>(allocatedMemory);
				entry.allocationSize = sizeToAllocate;
				entry.pAllocationFile = pFileName;
				entry.lineNumber = lineNum;
				return allocatedMemory;
			}
		}

		EXE_ASSERT(false); // We have allocated and traced more memory than we currently allow.
		return allocatedMemory;
	}

	void TraceAllocator::Free(void* memoryToFree, size_t sizeToFree, bool)
	{
		if (!memoryToFree)
			return;

		// O(n) loop through array. I could take the BleachLeakDetector route and use a
		// hashmap and hash the line+filename, but that would also allocate memory in some cases.
		// This route will never allocate memory while this tracer is in effect.
		for (auto& entry : m_trackedMemory)
		{
			if (entry.memoryAddress == reinterpret_cast<uintptr_t>(memoryToFree))
			{
				// We only track the memory we allocated.
				++m_deallocationCount;
				if (sizeToFree <= 0)
					m_totalDeallocatedBytes += entry.allocationSize;
				else
					m_totalDeallocatedBytes += sizeToFree;

				// Zero out the entry.
				entry.memoryAddress = 0;
				entry.pAllocationFile = nullptr;
				entry.lineNumber = 0;
				entry.allocationSize = 0;

				// This was one of Exelius's allocations, so we can free it (aligned).
				m_pParentAllocator->Free(memoryToFree, sizeToFree, true);
				return;
			}
		}

		EXE_ASSERT(m_pParentAllocator);
		m_pParentAllocator->Free(memoryToFree, sizeToFree);
	}

	void TraceAllocator::DumpMemoryData()
	{
		std::cout << "\n----------------------------------------------------\n";
		std::cout << "Debug Memory Manager Data Dump";
		std::cout << "\n----------------------------------------------------\n";
		std::cout << "Current Total Allocation Count: " << m_allocationCount << "\n";
		std::cout << "Current Total Deallocation Count: " << m_deallocationCount << "\n";
		std::cout << "Difference: " << m_allocationCount - m_deallocationCount << "\n";

		std::cout << "Current Total Memory Allocated: " << m_totalAllocatedBytes << "\n";
		std::cout << "Current Total Memory Deallocated: " << m_totalDeallocatedBytes << "\n";
		std::cout << "Difference: " << m_totalAllocatedBytes - m_totalDeallocatedBytes << "\n";

		for (auto& entry : m_trackedMemory)
		{
			if (entry.memoryAddress != 0)
			{
				if (entry.pAllocationFile)
				{
					std::cout << "Leak Detected: Address: " << entry.memoryAddress << ", Size (bytes): " << entry.allocationSize  << ", Filename: " << entry.pAllocationFile << ", Line Number: " << entry.lineNumber << "\n";
				}
			}
		}
	}
}