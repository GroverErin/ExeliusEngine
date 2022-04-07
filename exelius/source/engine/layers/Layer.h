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
		// TODO: Consider stripping this from release builds.
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

		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event&) {}

		// TODO: Consider stripping this from release builds.
		inline const eastl::string& GetLayerName() const { return m_layerName; }
	};
}