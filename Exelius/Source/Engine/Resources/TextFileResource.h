#pragma once

#include "Source/Resource/Resource.h"

#include <EASTL/string.h>
namespace Exelius
{
	class TextFileResource
		: public Resource
	{
		eastl::string m_text;
	public:
		TextFileResource(const ResourceID& id);
		TextFileResource(const TextFileResource&) = delete;
		TextFileResource(TextFileResource&&) = delete;
		TextFileResource& operator=(const TextFileResource&) = delete;
		virtual ~TextFileResource() = default;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override {}

		const eastl::string& GetText() const { return m_text; }
	};
}