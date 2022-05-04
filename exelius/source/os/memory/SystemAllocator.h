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
		virtual void* Allocate(size_t sizeToAllocate, size_t /* memoryAlignment */, const char* /* pFileName */, int /* lineNum */) final override
		{
		//	// https://stackoverflow.com/questions/3839922/aligned-malloc-in-gcc
		//#ifdef EXE_WINDOWS
		//	return _aligned_malloc(sizeToAllocate, memoryAlignment);
		//#else
		//	return aligned_alloc(memoryAlignment, sizeToAllocate);
		//#endif // EXE_WINDOWS
			void* ptr = malloc(sizeToAllocate);
			EXE_ASSERT(ptr);
			if (!ptr)
				return nullptr;
			memset(ptr, 0, sizeToAllocate);
			return ptr;
		}

		virtual void Free(void* memoryToFree, size_t /* sizeToFree */, bool /* isAligned */) final override
		{
		//	// Not sure if this is the correct way to get away from the
		//	// "The block at 0x%p was not allocated by _aligned routines, use free()"
		//	// error.
		//	if (!isAligned)
		//	{
		//		free(memoryToFree);
		//		return;
		//	}

		//	// https://stackoverflow.com/questions/3839922/aligned-malloc-in-gcc
		//#ifdef EXE_WINDOWS
		//	_aligned_free(memoryToFree);
		//#else
		//	free(memoryToFree);
		//#endif // EXE_WINDOWS

			if (memoryToFree)
				free(memoryToFree);
		}
	};
}