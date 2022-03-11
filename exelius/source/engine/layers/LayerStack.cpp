#include "EXEPCH.h"
#include "LayerStack.h"
#include "Layer.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	LayerStack::~LayerStack()
	{
		for (Layer* pLayer : m_layers)
		{
			pLayer->OnDetach();
			EXELIUS_DELETE(pLayer);
		}
	}

	void LayerStack::PushLayer(Layer* pLayer)
	{
		EXE_ASSERT(pLayer);

		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, pLayer);
		++m_layerInsertIndex;
		pLayer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* pLayer)
	{
		EXE_ASSERT(pLayer);

		auto found = eastl::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, pLayer);
		if (found != m_layers.begin() + m_layerInsertIndex)
		{
			pLayer->OnDetach();
			m_layers.erase(found);
			--m_layerInsertIndex;
		}
	}

	void LayerStack::PushOverlayLayer(Layer* pOverlayLayer)
	{
		EXE_ASSERT(pOverlayLayer);

		m_layers.emplace_back(pOverlayLayer);
		pOverlayLayer->OnAttach();
	}

	void LayerStack::PopOverlayLayer(Layer* pOverlayLayer)
	{
		EXE_ASSERT(pOverlayLayer);

		auto found = eastl::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), pOverlayLayer);
		if (found != m_layers.end())
		{
			pOverlayLayer->OnDetach();
			m_layers.erase(found);
		}
	}
}