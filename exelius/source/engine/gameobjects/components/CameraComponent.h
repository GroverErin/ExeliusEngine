#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/render/camera/SceneCamera.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct CameraComponent
		: public Component
	{
		SceneCamera m_camera;
		glm::vec4 m_viewportRect = { 0.0f, 0.0f, 1.0f, 1.0f };
		bool m_isActive = true;
		bool m_isFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		virtual void OnComponentAdded(Scene* pOwningScene) final override;

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;
	};
}