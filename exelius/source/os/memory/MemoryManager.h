#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/os/memory/SystemAllocator.h"
#include "source/os/memory/TraceAllocator.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class MemoryManager
		: public Singleton<MemoryManager>
	{
		SystemAllocator m_systemAllocator;	// Root allocator, calls malloc/free.
		TraceAllocator m_traceAllocator;	// Optional Debug Wrapper for root allocator.

		ExeliusAllocator* m_pGlobalAllocator;
	public:
		void Initialize(bool useTraceAllocator)
		{
			m_traceAllocator.SetParentAllocator(&m_systemAllocator);

			if (useTraceAllocator)
				m_pGlobalAllocator = &m_systemAllocator;
			else
				m_pGlobalAllocator = &m_traceAllocator;
		}

		ExeliusAllocator* GetGlobalAllocator() { return m_pGlobalAllocator; }
	};
}