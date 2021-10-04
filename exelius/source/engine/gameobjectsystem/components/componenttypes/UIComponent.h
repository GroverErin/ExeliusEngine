#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"
#include "source/utility/math/Rectangle.h"
#include "source/render/RenderCommand.h"

#include <EASTL/array.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIComponent
		: public Component
	{
		ResourceID m_textureID;

		eastl::array<RenderCommand, 9> m_commands;
		Vector2f m_dimensions;
	public:
		DEFINE_COMPONENT(UIComponent);

		UIComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			//
		}

		virtual bool Initialize() final override;
		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Render() final override;

		virtual void Destroy() final override;

		float GetW() const { return m_dimensions.w; }
		void SetW(float w);
		float GetH() const { return m_dimensions.h; }
		void SetH(float h);

		void Reset();
	};
}