#include "EXEPCH.h"
#include "source/os/memory/MemoryOverloads.h"
#include "source/os/memory/MemoryManager.h"

void* operator new(size_t sizeToAllocate, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, 16, pFileName, lineNum);
}

void* operator new[](size_t sizeToAllocate, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, 16, pFileName, lineNum);
}

void* operator new(size_t sizeToAllocate, std::align_val_t memoryAlignment, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, (size_t)memoryAlignment, pFileName, lineNum);
}

void* operator new[](size_t sizeToAllocate, std::align_val_t memoryAlignment, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, (size_t)memoryAlignment, pFileName, lineNum);
}

void* operator new(size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, memoryAlignment, pFileName, lineNum);
}

void* operator new[](size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    return pGlobalAllocator->Allocate(sizeToAllocate, memoryAlignment, pFileName, lineNum);
}

void operator delete(void* pMemoryToFree)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete[](void* pMemoryToFree)
{
    auto pMemManager = Exelius::MemoryManager::GetInstance();
    EXE_ASSERT(pMemManager);
    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    EXE_ASSERT(pGlobalAllocator);
    pGlobalAllocator->Free(pMemoryToFree);
}
