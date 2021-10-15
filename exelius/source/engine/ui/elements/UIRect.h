#pragma once
#include "source/engine/ui/UIElement.h"
#include "source/utility/generic/Color.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIRect
		: public UIElement
	{
	public:

		UIRect(UIElement* pParent = nullptr)
			: UIElement(pParent)
		{
			//
		}

		virtual bool Initialize(const rapidjson::Value& jsonUIElementData) final override;

		virtual void OnRender() final override;
	};
}