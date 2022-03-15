#pragma once
#include "include/Exelius.h"

#include <glm/glm.hpp>

class EditorLayer : public Exelius::Layer
{
	Exelius::OrthographicCameraController m_CameraController;

	// Temp
	Exelius::SharedPtr<Exelius::VertexArray> m_SquareVA;
	Exelius::SharedPtr<Exelius::Shader> m_FlatColorShader;

	Exelius::SharedPtr<Exelius::Texture> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	void OnEvent(Exelius::Event & e) override;
};
