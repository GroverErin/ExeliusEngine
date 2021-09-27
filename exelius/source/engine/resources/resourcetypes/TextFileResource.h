#pragma once

#include "source/resource/Resource.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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

		const eastl::string& GetRawText() const { return m_text; }
	};
}