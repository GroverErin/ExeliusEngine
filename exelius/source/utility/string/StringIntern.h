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

		// Using a pointer here because hash_set allocates memory
		// and will cause the Memory Tracker to pick up false positive
		// leaks unless deleted before the data dump at engine shutdown.
		inline static StringInternSet* s_pStringSet = nullptr;

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

		StringIntern(char character)
			: m_pString(nullptr)
		{
			FindOrAdd(&character);
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

		bool IsValid() const { return m_pString != nullptr; }

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


		/// <summary>
		/// Clears the string intern set. This is an Exelius internal
		/// function. DO NOT call this function, as the engine relies
		/// on the string intern set.
		/// 
		/// This function is here to deallocate the memory of the set
		/// upon engine shutdown.
		/// </summary>
		static void _ClearStringInternSet()
		{
			if (s_pStringSet)
				s_pStringSet->clear();

			EXELIUS_DELETE(s_pStringSet);
		}

	private:
		void Set(const StringIntern& stringIntern)
		{
			m_pString = stringIntern.m_pString;
		}

		void FindOrAdd(const eastl::string& string)
		{
			if (!s_pStringSet)
				s_pStringSet = EXELIUS_NEW(StringInternSet());

			auto found = s_pStringSet->find(string);
			if (found != s_pStringSet->end())
			{
				// Assign it the same data.
				m_pString = &(*found);
			}
			else
			{
				// Add it to the set.
				m_pString = &(*s_pStringSet->emplace(string).first);
			}
		}

		void FindOrAdd(eastl::string&& string)
		{
			if (!s_pStringSet)
				s_pStringSet = EXELIUS_NEW(StringInternSet());

			// TODO: Access needs to be wrapping in a mutex
			auto found = s_pStringSet->find(string);
			if (found != s_pStringSet->end())
			{
				// Assign it the same data.
				m_pString = &(*found);
			}
			else
			{
				// Add it to the set.
				m_pString = &(*s_pStringSet->emplace(std::move(string)).first); //<-- STD::MOVE
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