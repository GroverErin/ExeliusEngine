#pragma once
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentFactory.h"

namespace Exelius
{
	class ExeliusComponentFactory
		: public ComponentFactory
	{
	public:
		ExeliusComponentFactory() = default;
		ExeliusComponentFactory(const ExeliusComponentFactory&) = delete;
		ExeliusComponentFactory(ExeliusComponentFactory&&) = delete;
		ExeliusComponentFactory& operator=(const ExeliusComponentFactory&) = delete;
		ExeliusComponentFactory& operator=(ExeliusComponentFactory&&) = delete;
		virtual ~ExeliusComponentFactory() = default;

		virtual bool Initialize() override;
		virtual Handle CreateComponent(const Component::Type& componentName, GameObject* pOwningObject, const rapidjson::Value& componentData) override;
	};
}