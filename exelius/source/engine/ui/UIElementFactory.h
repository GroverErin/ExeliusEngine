#pragma once
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class UIElement;
	class UIElementFactory
	{
	public:
		static UIElement* CreateUIElement(const eastl::string& elementType, UIElement* pParent = nullptr);
	};
}