#pragma once
#include "source/utility/generic/Macros.h"
#include <EASTL/hash_set.h>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class StringIntern
	{
		friend bool operator<(const StringIntern& left, const StringIntern& right);

		// Using a hash set here because it only requires 1 value, not a key value pair.
		using StringInternSet = eastl::hash_set<eastl::string>;

		// why does not inlining cause build error??
		inline static StringInternSet s_stringSet = StringInternSet();
		const eastl::string* m_pString;

	public:
		StringIntern()
			: m_pString(nullptr)
		{
			//
		}

		StringIntern(const eastl::string& string)
			: m_pString(nullptr)
		{
			FindOrAdd(string);
		}

		StringIntern(const char* pString)
			: m_pString(nullptr)
		{
			FindOrAdd(pString);
		}

		StringIntern(const StringIntern& stringIntern)
			: m_pString(nullptr)
		{
			Set(stringIntern);
		}

		StringIntern(eastl::string&& string)
			: m_pString(nullptr)
		{
			FindOrAdd(::std::move(string));
		}

		StringIntern(StringIntern&&) = default;

		const eastl::string& Get() const { EXE_ASSERT(IsValid()); return (*m_pString); }

		bool IsValid() const
		{
			if (m_pString != nullptr)
				return m_pString != nullptr;
			
			return m_pString != nullptr;
		}

		StringIntern& operator=(const StringIntern& right)
		{
			if (this == &right)
				return *this;

			Set(right);
			return (*this);
		}

		StringIntern& operator=(StringIntern&& right) noexcept
		{
			if (this == &right)
				return *this;

			Set(right);
			return (*this);
		}

		StringIntern& operator=(const eastl::string& right) { FindOrAdd(right); return (*this); }
		StringIntern& operator=(eastl::string&& right) { FindOrAdd(std::move(right)); return (*this); }
		StringIntern& operator=(const char* right) { FindOrAdd(right); return (*this); }
		StringIntern& operator+=(const eastl::string& right) { FindOrAdd((*m_pString)+right); return (*this); }

		bool operator==(const StringIntern& right) const { return m_pString == right.m_pString; }
		bool operator!=(const StringIntern& right) const { return m_pString != right.m_pString; }
		bool operator==(const eastl::string& right) const { return (*m_pString) == right; }
		bool operator!=(const eastl::string& right) const { return (*m_pString) != right; }
		bool operator==(const char* pRight) const { return ::strcmp(m_pString->c_str(), pRight) == 0; }
		bool operator!=(const char* pRight) const { return ::strcmp(m_pString->c_str(), pRight) != 0; }

		operator const char* () const { EXE_ASSERT(m_pString); return (m_pString->c_str()); }
		operator const eastl::string&() const { EXE_ASSERT(m_pString); return (*m_pString); }

	private:
		void Set(const StringIntern& stringIntern)
		{
			m_pString = stringIntern.m_pString;
		}

		void FindOrAdd(const eastl::string& string)
		{
			auto found = s_stringSet.find(string);
			if (found != s_stringSet.end())
			{
				// Assign it the same data.
				m_pString = &(*found);
			}
			else
			{
				// Add it to the set.
				m_pString = &(*s_stringSet.emplace(string).first);
			}
		}

		void FindOrAdd(eastl::string&& string)
		{
			// TODO: Access needs to be wrapping in a mutex
			auto found = s_stringSet.find(string);
			if (found != s_stringSet.end())
			{
				// Assign it the same data.
				m_pString = &(*found);
			}
			else
			{
				// Add it to the set.
				m_pString = &(*s_stringSet.emplace(std::move(string)).first); //<-- STD::MOVE
			}
		}

	};

	inline bool operator<(const Exelius::StringIntern& left, const Exelius::StringIntern& right)
	{
		return (left.m_pString < right.m_pString);
	}

}

namespace eastl
{
	template<>
	struct hash<Exelius::StringIntern>
	{
	public:
		// Used for storing a StringIntern as a key in EASTL hash maps.
		size_t operator()(const Exelius::StringIntern& key) const noexcept
		{
			const char* stringPointer = key.Get().c_str();
			const eastl::hash<const char8_t*> hashFunction;
			return hashFunction(stringPointer);
		}
	};
}