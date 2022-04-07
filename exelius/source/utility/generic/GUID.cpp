#include "EXEPCH.h"
#include "GUID.h"

#include "source/utility/random/Random.h"

#include <time.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	GUID::GUID()
		: m_ID(0)
	{
		Random rand(time(nullptr), time(nullptr));
		m_ID = rand.Rand();
	}
	
	GUID::GUID(uint64_t id)
		: m_ID(id)
	{
		//
	}
}