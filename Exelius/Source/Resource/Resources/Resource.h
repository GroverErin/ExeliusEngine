#pragma once
#include "Source/Utility/Math/Math.h"
#include <functional>

namespace Exelius
{
	class Resource
	{
		unsigned int m_id;
		std::string m_name;
	public:

		unsigned int GetID() { return m_id; }
		std::string& GetName() { return m_name; }

		virtual void Load() = 0;
		virtual void Unload() = 0;

	protected:
		Resource(std::string& resourceName)
			: m_id(0)
			, m_name(resourceName)
		{
			m_id = HashString(resourceName);
		}
	};

	using ResourcePtr = std::shared_ptr<Resource>;
}