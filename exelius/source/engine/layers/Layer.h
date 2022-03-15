#pragma once
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Event;

	class Layer
	{
	protected:
		eastl::string m_layerName;
	public:
		Layer(const eastl::string& layerName = "Unnamed Layer")
			: m_layerName(layerName)
		{
			//
		}

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		// On Update and On Render are not necessary, but helps with organization.
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event&) {}

		inline const eastl::string& GetLayerName() const { return m_layerName; }
	};
}