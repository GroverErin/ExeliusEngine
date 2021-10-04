#include "EXEPCH.h"
#include "source/os/memory/MemoryOverloads.h"
#include "source/os/memory/MemoryManager.h"

void* operator new(size_t sizeToAllocate, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, pFileName, lineNum);
}

void operator delete(void* pMemoryToFree, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    pGlobalAllocator->Free(pMemoryToFree);
}

void* operator new[](size_t sizeToAllocate, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, pFileName, lineNum);
}

void operator delete[](void* pMemoryToFree, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    pGlobalAllocator->Free(pMemoryToFree);
}
