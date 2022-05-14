#pragma once
#include "source/engine/layers/Layer.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ImGuiLayer final : public Layer
	{
		bool m_blockEvents;
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() final override;
		virtual void OnDetach() final override;
		virtual void OnEvent(Event& evnt) final override;

		bool Begin();
		void End();

		void ShouldBlockEvents(bool shouldBlock)
		{
			m_blockEvents = shouldBlock;
		}
	};
}