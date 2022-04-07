#pragma once

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Layer;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* pLayer);
		void PopLayer(Layer* pLayer);

		void PushOverlayLayer(Layer* pOverlayLayer);
		void PopOverlayLayer(Layer* pOverlayLayer);

		void ClearLayerStack();

		// Allow iteration over the "stack"
		eastl::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		eastl::vector<Layer*>::iterator end() { return m_layers.end(); }

		eastl::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		eastl::vector<Layer*>::const_iterator end()	const { return m_layers.end(); }

		eastl::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		eastl::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		eastl::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		eastl::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }
	private:
		eastl::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex;
	};
}