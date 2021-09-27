#pragma once
#include "source/utility/generic/Singleton.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ExeliusAllocator
		: public Singleton<ExeliusAllocator>
	{
	public:
		virtual void* Allocate(size_t sizeToAllocate) = 0;
		virtual void Free(void* memoryToFree, size_t sizeToFree = 0) = 0;
	};
}