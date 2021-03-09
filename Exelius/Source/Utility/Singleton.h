#pragma once

#include "Source/Utility/Macros.h"
#include <assert.h>

namespace Exelius
{

    template <class Type>
    class Singleton
    {
        static Type* s_pSingleton;

    public:
        static void SetSingleton(Type* pSingleton) { assert(s_pSingleton == nullptr); s_pSingleton = pSingleton; }
        static void DestroySingleton() { SAFE_DELETE(s_pSingleton); }
        static Type* GetInstance() { return s_pSingleton; }
    };

    template <class Type>
    Type* Singleton<Type>::s_pSingleton = nullptr;
}