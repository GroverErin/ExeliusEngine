#pragma once
#include "Source/Engine/GameObjectSystem/Components/Component.h"
#include "Source/Resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIComponent
		: public Component
	{
		ResourceID m_textureID;
	public:
		DEFINE_COMPONENT(UIComponent);

		UIComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			//
		}

		virtual bool Initialize() final override;
		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Render() const final override;

		virtual void Destroy() final override;
	};
}