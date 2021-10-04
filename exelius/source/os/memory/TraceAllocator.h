#pragma once
#include "source/os/memory/ExeliusAllocator.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Log;

	class TraceAllocator
		: public ExeliusAllocator
	{
		ExeliusAllocator* m_pParentAllocator;
		Log* m_pMemoryLog;

		int32_t m_allocationCount;
		size_t m_totalAllocatedBytes;

	public:
		TraceAllocator();

		TraceAllocator(ExeliusAllocator* pParentAllocator);

		virtual ~TraceAllocator();

		void SetParentAllocator(ExeliusAllocator* pParentAllocator);

		virtual void* Allocate(size_t sizeToAllocate, const char* pFileName, int lineNum) final override;

		virtual void Free(void* memoryToFree, size_t sizeToFree) final override;

		virtual void DumpMemoryData() final override;
	};
}