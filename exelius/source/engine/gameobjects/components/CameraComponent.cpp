#include "EXEPCH.h"
#include "CameraComponent.h"
#include "source/engine/scenesystem/Scene.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void CameraComponent::OnComponentAdded(Scene* pOwningScene)
	{
		uint32_t width = pOwningScene->GetViewportWidth();
		uint32_t height = pOwningScene->GetViewportHeight();

		width = (uint32_t)(width * m_viewportRect.z);
		height = (uint32_t)(height * m_viewportRect.w);

		if (width > 0 && height > 0)
			m_camera.SetViewportSize(width, height);
	}

	void CameraComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("CameraComponent");
		writer.StartObject(); // Start Camera Component.
		{
			writer.Key("ProjectionType");
			writer.Int((int)m_camera.GetProjectionType());

			writer.Key("PerspectiveFOV");
			writer.Double((double)m_camera.GetPerspectiveVerticalFOV());

			writer.Key("PerspectiveNear");
			writer.Double((double)m_camera.GetPerspectiveNearClip());

			writer.Key("PerspectiveFar");
			writer.Double((double)m_camera.GetPerspectiveFarClip());

			writer.Key("OrthographicSize");
			writer.Double((double)m_camera.GetOrthographicSize());

			writer.Key("OrthographicNear");
			writer.Double((double)m_camera.GetOrthographicNearClip());

			writer.Key("OrthographicFar");
			writer.Double((double)m_camera.GetOrthographicFarClip());

			writer.Key("IsActive");
			writer.Bool(m_isActive);

			writer.Key("IsPrimary");
			writer.Bool(m_isPrimary);

			writer.Key("IsFixedAspectRatio");
			writer.Bool(m_isFixedAspectRatio);

			writer.Key("ViewportRect");
			writer.StartArray();
			writer.Double((double)m_viewportRect.x);
			writer.Double((double)m_viewportRect.y);
			writer.Double((double)m_viewportRect.z);
			writer.Double((double)m_viewportRect.w);
			writer.EndArray();
		}
		writer.EndObject(); // End Camera Component.
	}
	
	void CameraComponent::DeserializeComponent(const rapidjson::Value& componentValue)
	{
		m_camera.SetProjectionType((SceneCamera::ProjectionType)componentValue.FindMember("ProjectionType")->value.GetInt());

		m_camera.SetPerspectiveVerticalFOV(componentValue.FindMember("PerspectiveFOV")->value.GetFloat());
		m_camera.SetPerspectiveNearClip(componentValue.FindMember("PerspectiveNear")->value.GetFloat());
		m_camera.SetPerspectiveFarClip(componentValue.FindMember("PerspectiveFar")->value.GetFloat());

		m_camera.SetOrthographicSize(componentValue.FindMember("OrthographicSize")->value.GetFloat());
		m_camera.SetOrthographicNearClip(componentValue.FindMember("OrthographicNear")->value.GetFloat());
		m_camera.SetOrthographicFarClip(componentValue.FindMember("OrthographicFar")->value.GetFloat());

		m_isActive = componentValue.FindMember("IsActive")->value.GetBool();
		if (componentValue.FindMember("IsPrimary") != componentValue.MemberEnd())
			m_isPrimary = componentValue.FindMember("IsPrimary")->value.GetBool();
		m_isFixedAspectRatio = componentValue.FindMember("IsFixedAspectRatio")->value.GetBool();

		if (componentValue.FindMember("ViewportRect") != componentValue.MemberEnd())
		{
			auto& viewportRect = componentValue.FindMember("ViewportRect")->value;
			EXE_ASSERT(viewportRect.IsArray());

			m_viewportRect.x = viewportRect.GetArray()[0].GetFloat();
			m_viewportRect.y = viewportRect.GetArray()[1].GetFloat();
			m_viewportRect.z = viewportRect.GetArray()[2].GetFloat();
			m_viewportRect.w = viewportRect.GetArray()[3].GetFloat();
		}
	}
}
