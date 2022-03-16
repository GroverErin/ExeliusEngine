#pragma once
#include "include/Exelius.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class EditorLayer : public Layer
	{
		OrthographicCameraController m_CameraController;

		// Temp
		SharedPtr<VertexArray> m_SquareVA;
		SharedPtr<Shader> m_FlatColorShader;
		SharedPtr<Framebuffer> m_FrameBuffer;

		SharedPtr<Texture> m_CheckerboardTexture;

		glm::vec2 m_viewportSize = { 0.0f, 0.0f };
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		bool m_viewportFocused = false;
		bool m_viewportHovered = false;
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate() override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	};
}