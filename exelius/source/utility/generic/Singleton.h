#pragma once
#include <source/utility/generic/Macros.h>
#include <source/os/memory/MemoryOverloads.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    template <class Type>
    class Singleton
    {
        static Type* s_pSingleton;

    public:
        static void SetSingleton(Type* pSingleton) { EXE_ASSERT(!s_pSingleton); s_pSingleton = pSingleton; }
        static void DestroySingleton() { EXELIUS_DELETE(s_pSingleton); }
        static Type* GetInstance() { return s_pSingleton; }
    };

    template <class Type>
    Type* Singleton<Type>::s_pSingleton = nullptr;
}