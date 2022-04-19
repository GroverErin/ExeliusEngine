#include "InspectorPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <cstring>
#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void InspectorPanel::OnImGuiRender(GameObject gameObject)
	{
		ImGui::Begin("Inspector");
		{
			if (gameObject)
				DrawComponents(gameObject);
		}
		ImGui::End();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, GameObject gameObject, UIFunction uiFunction, bool canBeRemoved = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (gameObject.HasComponent<T>())
		{
			auto& component = gameObject.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			if (canBeRemoved)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				gameObject.RemoveComponent<T>();
		}
	}

	void InspectorPanel::DrawComponents(GameObject gameObject)
	{
		if (!gameObject.HasComponent<NameComponent>())
		{
			EXE_LOG_CATEGORY_FATAL("ExeliusEditor", "GameObject found wth no NameComponent");
			EXE_ASSERT(false);
		}
		if (!gameObject.HasComponent<GUIDComponent>())
		{
			EXE_LOG_CATEGORY_FATAL("ExeliusEditor", "GameObject found wth no GUIDComponent");
			EXE_ASSERT(false);
		}
		if (!gameObject.HasComponent<TransformComponent>())
		{
			EXE_LOG_CATEGORY_FATAL("ExeliusEditor", "GameObject found wth no TransformComponent");
			EXE_ASSERT(false);
		}

		eastl::string& name = gameObject.GetComponent<NameComponent>().m_name;

		char buffer[256]; // TODO: This limit should be imposed in the actual component.
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, name.c_str(), sizeof(buffer));

		// Take up the entire width.
		ImGui::PushItemWidth(-1);

		// ImGui hides anything after '##'
		if (ImGui::InputText("##NameComponent", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
			gameObject.GetComponent<NameComponent>().m_name = eastl::string(buffer);
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", gameObject, [&](TransformComponent& component)
			{
				DrawVector3("Translation", component.m_translation);
				glm::vec3 rotation = glm::degrees(component.m_rotation);
				DrawVector3("Rotation", rotation);
				component.m_rotation = glm::radians(rotation);
				DrawVector3("Scale", component.m_scale);
			}, false);

		DrawComponent<CameraComponent>("Camera", gameObject, [](CameraComponent& component)
			{
				auto& camera = component.m_camera;

				ImGui::Checkbox("Is Active", &component.m_isActive);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.m_isFixedAspectRatio);
				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", gameObject, [](SpriteRendererComponent& component)
			{
				glm::vec4 color = component.m_color.GetColorVector();
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				component.m_color = color;

				char buffer[256]; // TODO: This limit should be imposed in the actual component.
				memset(buffer, 0, sizeof(buffer));
				if (component.m_textureResource.IsReferenceHeld())
					std::strncpy(buffer, component.m_textureResource.GetID().Get().c_str(), sizeof(buffer));

				if (ImGui::InputText("Texture", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.m_textureResource.SetResourceID(buffer);
					component.m_textureResource.LoadNow(); // Should Acquire here.
				}

				ImGui::DragFloat("Tiling Multiplier", &component.m_textureTilingMultiplier, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", gameObject, [](CircleRendererComponent& component)
			{
				glm::vec4 color = component.m_color.GetColorVector();
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				component.m_color = color;

				ImGui::DragFloat("Thickness", &component.m_thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.m_fade, 0.00025f, 0.0f, 1.0f);
			});

		DrawComponent<RigidbodyComponent>("Rigidbody", gameObject, [](RigidbodyComponent& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.m_bodyType];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.m_bodyType = (RigidbodyComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Is Fixed Rotation", &component.m_isFixedRotation);
			});

		DrawComponent<BoxColliderComponent>("Box Collider", gameObject, [](BoxColliderComponent& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.m_offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.m_size));
				ImGui::DragFloat("Density", &component.m_density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.m_friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.m_restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.m_restitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<CircleColliderComponent>("Circle Collider", gameObject, [](CircleColliderComponent& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.m_offset));
				ImGui::DragFloat("Radius", &component.m_radius);
				ImGui::DragFloat("Density", &component.m_density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.m_friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.m_restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.m_restitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<LuaScriptComponent>("Lua Script", gameObject, [](LuaScriptComponent& component)
			{
				char buffer[256]; // TODO: This limit should be imposed in the actual component.
				memset(buffer, 0, sizeof(buffer));
				if (component.m_scriptResource.IsReferenceHeld())
					std::strncpy(buffer, component.m_scriptResource.GetID().Get().c_str(), sizeof(buffer));

				if (ImGui::InputText("Script", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.m_scriptResource.SetResourceID(buffer);
					component.m_scriptResource.LoadNow(); // Should Acquire here.
				}
			});

		ImGui::Separator();

		DrawAddComponentPanel(gameObject);
	}
	
	void InspectorPanel::DrawAddComponentPanel(GameObject gameObject)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		float addComponentButtonSize = ImGui::CalcTextSize("Add Component").x + style.FramePadding.x * 2.0f;
		float availableContentRegion = ImGui::GetContentRegionAvail().x;

		float componentButtonOffset = (availableContentRegion - addComponentButtonSize) * 0.5f;
		float centerPosOffset = ImGui::GetCursorPosX() + componentButtonOffset;
		if (componentButtonOffset > 0.0f)
			ImGui::SetCursorPosX(centerPosOffset);

		static bool drawAddComponentList = false;
		if (ImGui::Button("Add Component"))
			drawAddComponentList = !drawAddComponentList;

		if (!drawAddComponentList)
			return;

		static ImGuiTextFilter filter;

		float filterOffset = (availableContentRegion * 0.25f);
		float filterPosOffset = ImGui::GetCursorPosX() + filterOffset;
		if (filterOffset > 0.0f)
			ImGui::SetCursorPosX(filterPosOffset);

		filter.Draw("##FilterInput", availableContentRegion * 0.5f);

		ImGui::SetNextItemWidth(availableContentRegion * 0.5f);
		if (filterOffset > 0.0f)
			ImGui::SetCursorPosX(filterPosOffset);

		if (!ImGui::BeginListBox("##ComponentListBox"))
			return;

		static int selectedIndex = -1; // Here we store our selection data as an index.
		eastl::vector<eastl::pair<eastl::string, eastl::function<void(GameObject)>>> componentVector;

		if (!gameObject.HasComponent<CameraComponent>())
			componentVector.emplace_back("Camera", [](GameObject gameObject)
				{
					gameObject.AddComponent<CameraComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<SpriteRendererComponent>())
			componentVector.emplace_back("Sprite Renderer", [](GameObject gameObject)
				{
					gameObject.AddComponent<SpriteRendererComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<CircleRendererComponent>())
			componentVector.emplace_back("Circle Renderer", [](GameObject gameObject)
				{
					gameObject.AddComponent<CircleRendererComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<RigidbodyComponent>())
			componentVector.emplace_back("Rigidbody", [](GameObject gameObject)
				{
					gameObject.AddComponent<RigidbodyComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<BoxColliderComponent>())
			componentVector.emplace_back("Box Collider", [](GameObject gameObject)
				{
					gameObject.AddComponent<BoxColliderComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<CircleColliderComponent>())
			componentVector.emplace_back("Circle Collider", [](GameObject gameObject)
				{
					gameObject.AddComponent<CircleColliderComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		if (!gameObject.HasComponent<LuaScriptComponent>())
			componentVector.emplace_back("Lua Script", [](GameObject gameObject)
				{
					gameObject.AddComponent<LuaScriptComponent>();
					drawAddComponentList = false;
					selectedIndex = -1;
				});

		for (size_t i = 0; i < componentVector.size(); i++)
		{
			const bool isSelected = (selectedIndex == (int)i);

			if (!filter.PassFilter(componentVector[i].first.c_str()))
				continue;

			if (ImGui::Selectable(componentVector[i].first.c_str(), isSelected))
				selectedIndex = (int)i;

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
				if (componentVector[i].second)
					componentVector[i].second(gameObject);
			}
		}
		ImGui::EndListBox();
	}

	void InspectorPanel::DrawVector3(const eastl::string& label, glm::vec3& values, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::Text(" X ");

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text(" Y ");

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text(" Z ");

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}
