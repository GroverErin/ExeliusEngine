#pragma once
#include <cstdint>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// String hash functions from:
	/// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
	/// </summary>
	class StringHash
	{
		static constexpr uint32_t s_kOffsetBasis32		= 0x811c9dc5;
		static constexpr uint32_t s_kPrimeMultiplier32	= 0x1000193;
		static constexpr uint64_t s_kOffsetBasis64		= 0xcbf29ce484222325ull;
		static constexpr uint64_t s_kPrimeMultiplier64	= 0x100000001b3ull;

		template <class IntType, IntType kOffsetBasis, IntType kPrimeMultiplier>
		static constexpr IntType Fnv1aHash(const char* pStringToHash)
		{
			if (!pStringToHash)
				return kOffsetBasis;

			IntType hash = kOffsetBasis;
			while (*pStringToHash != '\0')
			{
				hash ^= static_cast<IntType>(*pStringToHash);
				hash *= kPrimeMultiplier;
				++pStringToHash;
			}

			return hash;
		}

	public:

		static constexpr uint32_t HashString32(const char* pStringToHash)
		{
			return Fnv1aHash<uint32_t, s_kOffsetBasis32, s_kPrimeMultiplier32>(pStringToHash);
		}

		static constexpr uint32_t HashString32(const eastl::string& stringToHash)
		{
			return Fnv1aHash<uint32_t, s_kOffsetBasis32, s_kPrimeMultiplier32>(stringToHash.c_str());
		}

		static constexpr uint64_t HashString64(const char* pStringToHash)
		{
			return Fnv1aHash<uint64_t, s_kOffsetBasis64, s_kPrimeMultiplier64>(pStringToHash);
		}

		static constexpr uint64_t HashString64(const eastl::string& stringToHash)
		{
			return Fnv1aHash<uint64_t, s_kOffsetBasis64, s_kPrimeMultiplier64>(stringToHash.c_str());
		}
	};
}