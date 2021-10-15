#include "EXEPCH.h"
#include "UIRect.h"

#include "source/render/RenderManager.h"

#include <EASTL/array.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool UIRect::Initialize(const rapidjson::Value& jsonUIElementData)
	{
		return UIElement::Initialize(jsonUIElementData);
	}

	void Exelius::UIRect::OnRender()
	{
		// TODO:
		//if (!isVisible)
		//	return;

		RenderCommand command;
		command.m_destinationFrame = m_actualRegion;
		command.m_tint = m_color;
		RenderManager::GetInstance()->PushRenderCommand(command);

		UIElement::OnRender();
	}
}
