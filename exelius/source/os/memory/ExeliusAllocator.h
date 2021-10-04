#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ExeliusAllocator
	{
	public:
		virtual void* Allocate(size_t sizeToAllocate, const char* pFileName, int lineNum) = 0;
		virtual void Free(void* pMemoryToFree, size_t sizeToFree = 0) = 0;

		virtual void DumpMemoryData() {};
	};
}