#pragma once
#include "source/os/memory/ExeliusAllocator.h"

#include <stdlib.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SystemAllocator
		: public ExeliusAllocator
	{
	public:
		virtual void* Allocate(size_t sizeToAllocate, const char* /* pFileName */, int /* lineNum */) final override
		{
			return malloc(sizeToAllocate);
		}

		virtual void Free(void* memoryToFree, size_t sizeToFree) final override
		{
			free(memoryToFree);
		}
	};
}