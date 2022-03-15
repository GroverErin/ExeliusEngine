#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"
#include "source/utility/math/Rectangle.h"

#include "source/engine/ui/UIElement.h"

#include <EASTL/array.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIComponent
		: public Component
	{
		UIElement m_uiRootElement;


		ResourceID m_textureID;
		Vector2f m_dimensions;
	public:
		DEFINE_COMPONENT(UIComponent);

		UIComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			//
		}

		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Update() final override;

		virtual void Render() final override;

		virtual void Destroy() final override;
	};
}