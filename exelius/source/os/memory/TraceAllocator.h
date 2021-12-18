#pragma once
#include "source/os/memory/ExeliusAllocator.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class TraceAllocator
		: public ExeliusAllocator
	{
		static constexpr uint32_t s_kMaxTrackedAllocationCount = 4096;
		ExeliusAllocator* m_pParentAllocator;

		int32_t m_allocationCount;
		size_t m_totalAllocatedBytes;
		int32_t m_deallocationCount;
		size_t m_totalDeallocatedBytes;

		/// <summary>
		/// The data we track for each memory allocation.
		/// 
		/// Size stats (I think):
		///		x86: 16 bytes
		///		x64: 28 bytes
		/// </summary>
		struct AllocationData
		{
			uintptr_t memoryAddress = 0;
			size_t allocationSize = 0;

			const char* pAllocationFile = nullptr;
			uint32_t lineNumber = 0;
		};

		/// <summary>
		/// Staticly allocated array of memory allocations.
		/// Currently the size of this array is:
		///		x86: 4098 x 16 =  65,568 bytes ||  65kB
		///		x64: 4098 x 28 = 114,744 bytes || 114kB
		/// </summary>
		AllocationData m_trackedMemory[s_kMaxTrackedAllocationCount];

	public:
		TraceAllocator();

		TraceAllocator(ExeliusAllocator* pParentAllocator);

		void SetParentAllocator(ExeliusAllocator* pParentAllocator);

		virtual void* Allocate(size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum) final override;

		virtual void Free(void* memoryToFree, size_t sizeToFree, bool) final override;

		virtual void DumpMemoryData() final override;
	};
}