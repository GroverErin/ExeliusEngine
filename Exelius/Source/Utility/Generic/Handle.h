#pragma once

#include <stdint.h>

#include <EASTL/functional.h>

namespace Exelius
{
	struct Handle
	{
		inline constexpr static uint64_t kIdFlag = 0x000000FFFFFFFF;

		uint64_t m_handle;

	public:
		Handle() : m_handle(0) {}
		Handle(uint64_t id) : m_handle(id) {}

		size_t GetId() const { return (size_t)(m_handle & kIdFlag); }

		uint32_t GetVersion() const { return (uint32_t)(m_handle >> 32); }

		// Note: Technically against GAP Standards. But don't wanna make a .cpp just for this line.
		void SetVersion(uint32_t version)
		{
			m_handle &= kIdFlag; // Clear Version
			m_handle |= ((uint64_t)version << 32); // Set Version 
		}

		// Note: See SetVersion.
		void Increment()
		{
			uint32_t version = GetVersion();
			SetVersion(++version);
		}

		bool IsValid() const { return GetVersion() != 0; }
		void Invalidate() { SetVersion(0); }

		operator bool() const { return GetVersion() != 0; }

		// Note: This is supposed to work instead of constructing the templated hash struct. But it doesn't.
		operator size_t() const { return GetId(); }

		bool operator==(const Handle& other) const { return m_handle == other.m_handle; }
		bool operator!=(const Handle& other) const { return !(*this == other); }
	};
}

// Custom specialization of hash.
namespace eastl
{
	template<> struct hash<Exelius::Handle>
	{
		size_t operator()(Exelius::Handle const& self) const noexcept
		{
			return self.GetId();
		}
	};
}