#pragma once

#include "Source/Resource/Resource.h"

#include <EASTL/string.h>
namespace Exelius
{
	class JSONResource
		: public Resource
	{
		eastl::string m_text;
	public:
		JSONResource(const ResourceID& id);
		JSONResource(const JSONResource&) = delete;
		JSONResource(JSONResource&&) = delete;
		JSONResource& operator=(const JSONResource&) = delete;
		virtual ~JSONResource() = default;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override {}

		const eastl::string& GetRawText() const { return m_text; }
	};
}