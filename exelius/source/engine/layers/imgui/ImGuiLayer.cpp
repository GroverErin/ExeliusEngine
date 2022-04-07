#include "EXEPCH.h"
#include "ImGuiLayer.h"

#include "source/engine/renderer/Renderer2D.h"

#include "source/os/events/Event.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

// TODO: Remove these
#include <GLFW/glfw3.h>
#include <glad/glad.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
		, m_blockEvents(true)
	{
		//
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows

		float fontSize = 18.0f; // TODO: Make Tunable
		io.Fonts->AddFontFromFileTTF("assets/fonts/audiowide.ttf", fontSize); // TODO: Make Tunable and use resource manager
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/audiowide.ttf", fontSize); // TODO: Make Tunable and use resource manager

		// Setup Dear ImGui style
		ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Set up some nice rounding.
		style.FrameRounding = 5.0f;
		style.PopupRounding = 5.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;

		OpenGLWindow& window = Renderer2D::GetInstance()->GetWindow().GetNativeWindow();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window.GetGLFWWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& evnt)
	{
		if (m_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			evnt.m_isHandled |= evnt.IsInCategory(EventCategory::Mouse) & io.WantCaptureMouse;
			evnt.m_isHandled |= evnt.IsInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		OpenGLWindow& window = Renderer2D::GetInstance()->GetWindow().GetNativeWindow();
		io.DisplaySize = ImVec2((float)window.GetWindowSize().w, (float)window.GetWindowSize().h);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
