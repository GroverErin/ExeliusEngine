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
		eastl::vector<char*> m_pooledMemory;

		eastl::stack<void*> m_freeMemory;

		// Internal block pointers.
		char* m_pAllocationFront;
		char* m_pAllocationCurrent;
		char* m_pAllocationEnd;


	public:
		PoolAllocator()
			: m_pAllocationFront(nullptr)
			, m_pAllocationCurrent(nullptr)
			, m_pAllocationEnd(nullptr)
		{
			AllocateNewChunk("PoolAllocatorConstructor", 0);
		}

		~PoolAllocator()
		{
			m_pAllocationFront = nullptr;
			m_pAllocationCurrent = nullptr;
			m_pAllocationEnd = nullptr;

			for (auto* pool : m_pooledMemory)
			{
				delete pool;
				pool = nullptr;
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

			m_pAllocationCurrent += sizeof(char*);
			return m_pAllocationCurrent - sizeof(char*);
		}

		virtual void Free(void* memoryToFree, size_t, bool) final override
		{
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
			m_pooledMemory.emplace_back((char*)pMem);

			m_pAllocationFront = (char*)pMem;
			m_pAllocationEnd = m_pAllocationFront + ChunkSize;
			m_pAllocationCurrent = m_pAllocationFront;
		}
	};
}