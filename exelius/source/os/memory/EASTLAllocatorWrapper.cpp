#include "EXEPCH.h"
#include "EASTLAllocatorWrapper.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	EASTLAllocatorWrapper::EASTLAllocatorWrapper(const char* pName)
		: m_pName(pName)
	{
	}

	void* EASTLAllocatorWrapper::allocate(size_t n, int /* flags = 0 */)
	{
		auto pMemManager = Exelius::MemoryManager::GetInstance();
		EXE_ASSERT(pMemManager);
		auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
		EXE_ASSERT(pGlobalAllocator);
		return pGlobalAllocator->Allocate(n, 16, m_pName, 0);
	}

	void* EASTLAllocatorWrapper::allocate(size_t n, size_t alignment, size_t /* offset */, int /* flags = 0 */)
	{
		auto pMemManager = Exelius::MemoryManager::GetInstance();
		EXE_ASSERT(pMemManager);
		auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
		EXE_ASSERT(pGlobalAllocator);
		return pGlobalAllocator->Allocate(n, alignment, m_pName, 0);
	}

	void EASTLAllocatorWrapper::deallocate(void* p, size_t n)
	{
		if (!p)
			return;

		auto pMemManager = Exelius::MemoryManager::GetInstance();
		if (!pMemManager)
			return free(p); // Triggers on Application Delete.

		auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
		if (!pGlobalAllocator)
			return free(p); // Should never trigger... *shrug*

		pGlobalAllocator->Free(p, n);
	}
}