#pragma once
#include "source/resource/ResourceFactory.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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

		virtual Resource* CreateResource(const ResourceID& resourceID) override;

	protected:
		virtual ResourceType::Type GetResourceType(const ResourceID& resourceID) const override;
	};
}