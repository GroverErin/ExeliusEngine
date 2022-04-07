#pragma once
#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class DebugPanel
	{
	public:

		void OnImGuiRender(GameObject hoveredGameObject);
	};
}