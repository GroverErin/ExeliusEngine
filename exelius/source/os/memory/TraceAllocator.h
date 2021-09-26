#pragma once
#include "Source/OS/Memory/ExeliusAllocator.h"
#include "Source/Debugging/Log.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class TraceAllocator
		: public ExeliusAllocator
	{
	public:
		virtual void* Allocate(size_t sizeToAllocate) final override
		{
			Log log("MemoryManager");
			log.Info("Allocating memory of size '{}'", sizeToAllocate);
			return malloc(sizeToAllocate);
		}

		virtual void Free(void* memoryToFree, size_t sizeToFree) final override
		{
			Log log("MemoryManager");
			log.Info("Freeing memory of size '{}'", sizeToFree);
			free(memoryToFree);
		}
	};
}