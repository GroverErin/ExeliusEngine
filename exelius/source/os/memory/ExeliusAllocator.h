#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ExeliusAllocator
	{
	public:
		virtual ~ExeliusAllocator() = default;

		virtual void* Allocate(size_t sizeToAllocate, size_t memoryAlignment = 16, const char* pFileName = nullptr, int lineNum = -1) = 0;
		virtual void Free(void* pMemoryToFree, size_t sizeToFree = 0, bool isAligned = false) = 0;

		virtual void DumpMemoryData() {};
	};
}