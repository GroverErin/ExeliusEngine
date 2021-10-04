#pragma once
#include <cstddef>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class EASTLAllocatorWrapper
	{
		const char* m_pName;
	public:
		EASTLAllocatorWrapper(const char* pName = nullptr);

		void* allocate(size_t n, int flags = 0);

		void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);

		void deallocate(void* p, size_t n);

		const char* get_name() const { return m_pName; }
		void set_name(const char* pName) { m_pName = pName; }
	};

	inline bool operator==(const EASTLAllocatorWrapper&, const EASTLAllocatorWrapper&) noexcept
	{
		return true;
	}

	inline bool operator!=(const EASTLAllocatorWrapper&, const EASTLAllocatorWrapper&) noexcept
	{
		return false;
	}

	inline static EASTLAllocatorWrapper s_EASTLAllocatorWrapper;
	inline static constexpr EASTLAllocatorWrapper* GetEASTLAllocatorWrapper() { return &s_EASTLAllocatorWrapper; }
}
