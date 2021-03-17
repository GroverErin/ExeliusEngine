#pragma once
#include "Source/Resource/ResourceFactory.h"

namespace Exelius
{
	class ExeliusResourceFactory
		: public ResourceFactory
	{
	public:
		ExeliusResourceFactory() = default;
		ExeliusResourceFactory(const ExeliusResourceFactory&) = delete;
		ExeliusResourceFactory(ExeliusResourceFactory&&) = delete;
		ExeliusResourceFactory& operator=(const ExeliusResourceFactory&) = delete;
		ExeliusResourceFactory& operator=(ExeliusResourceFactory&&) = delete;
		virtual ~ExeliusResourceFactory() = default;

		virtual Resource* CreateResource(const ResourceID& resourceID) final override;

	protected:
		virtual ResourceType::Type GetResourceType(const ResourceID& resourceID) const final override;
	};
}