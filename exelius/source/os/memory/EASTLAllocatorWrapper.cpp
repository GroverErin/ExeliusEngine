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

	void* EASTLAllocatorWrapper::allocate(size_t n, int)
	{
		EXE_ASSERT(MemoryManager::GetInstance());
		auto pAllocator = MemoryManager::GetInstance()->GetGlobalAllocator();
		EXE_ASSERT(pAllocator);
		return pAllocator->Allocate(n, m_pName, -1);
	}

	void* EASTLAllocatorWrapper::allocate(size_t n, size_t, size_t, int)
	{
		EXE_ASSERT(MemoryManager::GetInstance());
		auto pAllocator = MemoryManager::GetInstance()->GetGlobalAllocator();
		EXE_ASSERT(pAllocator);
		return pAllocator->Allocate(n, m_pName, -1);
	}

	void EASTLAllocatorWrapper::deallocate(void* p, size_t n)
	{
		EXE_ASSERT(MemoryManager::GetInstance());
		auto pAllocator = MemoryManager::GetInstance()->GetGlobalAllocator();
		EXE_ASSERT(pAllocator);
		pAllocator->Free(p, n);
	}
}