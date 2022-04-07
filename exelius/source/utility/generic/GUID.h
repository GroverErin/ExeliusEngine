#pragma once

#include <EASTL/hash_set.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GUID
	{
	public:
		GUID();
		GUID(uint64_t id);
		GUID(const GUID&) = default;

		operator uint64_t() const { return m_ID; }
	private:
		uint64_t m_ID;
	};
}

namespace eastl
{
	template<>
	struct hash<Exelius::GUID>
	{
	public:
		// Used for storing a GUID as a key in EASTL hash maps.
		size_t operator()(const Exelius::GUID& key) const noexcept
		{
			return hash<uint64_t>()((uint64_t)key);
		}
	};
}