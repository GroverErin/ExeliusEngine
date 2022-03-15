#pragma once
#include "source/engine/ui/UIElement.h"

#include "source/resource/ResourceHelpers.h"

#include <EASTL/array.h>
/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIImage
		: public UIElement
	{
		ResourceID m_textureID;
		Vector2f m_sourceSize;
	public:
		UIImage(UIElement* pParent = nullptr)
			: UIElement(pParent)
		{
			//
		}

		virtual bool Initialize(const rapidjson::Value& jsonUIElementData) final override;

		virtual void OnRender() final override;

		virtual void OnDestroy();

	private:
		void CalculateDestinationRects();
	};
}