#pragma once
#include "source/os/memory/ExeliusAllocator.h"
#include "source/os/memory/MemoryManager.h"

#include <EASTL/vector.h>
#include <EASTL/stack.h>
#include <new>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	template <size_t TypeSize, size_t ChunkSize>
	class PoolAllocator
		: public ExeliusAllocator
	{
		/// <summary>
		/// Vector of memory pages.
		/// </summary>
		eastl::vector<uintptr_t> m_pooledMemory;

		eastl::stack<void*> m_freeMemory;

		// Internal block pointers.
		uintptr_t m_pAllocationFront;
		uintptr_t m_pAllocationCurrent;
		uintptr_t m_pAllocationEnd;

	public:
		PoolAllocator()
			: m_pAllocationFront(0)
			, m_pAllocationCurrent(0)
			, m_pAllocationEnd(0)
		{
			AllocateNewChunk("PoolAllocatorConstructor", 0);
		}

		~PoolAllocator()
		{
			m_pAllocationFront = 0;
			m_pAllocationCurrent = 0;
			m_pAllocationEnd = 0;

			auto pMemManager = Exelius::MemoryManager::GetInstance();
			EXE_ASSERT(pMemManager);
			auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
			EXE_ASSERT(pGlobalAllocator);

			for (auto pool : m_pooledMemory)
			{
				pGlobalAllocator->Free(reinterpret_cast<void*>(pool));
				pool = 0;
			}
		}

		virtual void* Allocate(size_t sizeToAllocate, size_t , const char* pFileName, int lineNum) final override
		{
			if (!m_freeMemory.empty())
			{
				void* pMem = m_freeMemory.top();
				m_freeMemory.pop();
				return pMem;
			}

			if ((int)sizeToAllocate > m_pAllocationEnd - m_pAllocationCurrent)
			{
				// We've ran out of space in our current chunk.
				AllocateNewChunk(pFileName, lineNum);
			}

			m_pAllocationCurrent += TypeSize;
			return reinterpret_cast<void*>(m_pAllocationCurrent - TypeSize);
		}

		virtual void Free(void* memoryToFree, size_t, bool) final override
		{
			memset(memoryToFree, 0xCDCDCDCD, TypeSize);
			m_freeMemory.push(memoryToFree);
			memoryToFree = nullptr;
		}

	private:
		void AllocateNewChunk(const char* pFileName, int lineNum)
		{
			auto pMemManager = Exelius::MemoryManager::GetInstance();
			EXE_ASSERT(pMemManager);
			auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
			EXE_ASSERT(pGlobalAllocator);
			auto* pMem = pGlobalAllocator->Allocate(ChunkSize, 16U, pFileName, lineNum);
			m_pooledMemory.emplace_back(reinterpret_cast<uintptr_t>(pMem));

			m_pAllocationFront = reinterpret_cast<uintptr_t>(pMem);
			m_pAllocationEnd = m_pAllocationFront + ChunkSize;
			m_pAllocationCurrent = m_pAllocationFront;
		}
	};
}