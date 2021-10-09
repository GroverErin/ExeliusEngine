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
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete[](void* pMemoryToFree)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete(void* pMemoryToFree, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete[](void* pMemoryToFree, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete(void* pMemoryToFree, std::align_val_t, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete[](void* pMemoryToFree, std::align_val_t, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete(void* pMemoryToFree, size_t, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}

void operator delete[](void* pMemoryToFree, size_t, const char*, int)
{
    if (!pMemoryToFree)
        return;

    auto pMemManager = Exelius::MemoryManager::GetInstance();
    if (!pMemManager)
        return free(pMemoryToFree); // Triggers on Application Delete.

    auto pGlobalAllocator = pMemManager->GetGlobalAllocator();
    if (!pGlobalAllocator)
        return free(pMemoryToFree); // Should never trigger... *shrug*

    pGlobalAllocator->Free(pMemoryToFree);
}
