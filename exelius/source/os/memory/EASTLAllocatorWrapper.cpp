#include "EXEPCH.h"
#include "source/os/memory/ExeliusAllocator.h"
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
		auto* pAllocator = ExeliusAllocator::GetInstance();
		EXE_ASSERT(pAllocator);

		Log log("MemoryManager");
		log.Info("EASTL Allocating memory of size '{}'", n);
		return pAllocator->Allocate(n);
	}

	void* EASTLAllocatorWrapper::allocate(size_t n, size_t, size_t, int)
	{
		auto* pAllocator = ExeliusAllocator::GetInstance();
		EXE_ASSERT(pAllocator);

		Log log("MemoryManager");
		log.Info("EASTL Allocating memory of size '{}'", n);
		return pAllocator->Allocate(n);
	}

	void EASTLAllocatorWrapper::deallocate(void* p, size_t n)
	{
		auto* pAllocator = ExeliusAllocator::GetInstance();
		EXE_ASSERT(pAllocator);

		Log log("MemoryManager");
		log.Info("EASTL Freeing memory of size '{}'", n);
		pAllocator->Free(p, n);
	}
}