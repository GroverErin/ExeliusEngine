#pragma once
#include "source/os/events/EventManagement.h"

#include <EASTL/string.h>
#include <glm/vec2.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The properties of an Exelius::Window.
	/// This struct is used to initialize a new
	/// window.
	/// 
	/// Windows retain this class to contain the
	/// information about a window, and the data within
	/// will accurately reflect the state of the owning window.
	/// </summary>
	struct WindowProperties
	{
		/// <summary>
		/// The title of the window.
		/// </summary>
		eastl::string m_title = "Exelius Window";

		/// <summary>
		/// The size of the window.
		/// This is used to set the initial size of the window
		/// when fullscreen is not enabled intially. It will also
		/// contain the current size of a window after creation.
		/// </summary>
		glm::vec2 m_windowSize = { 720.0f, 640.0f };

		/// <summary>
		/// The position of the window.
		/// This is used to set the initial position of the window
		/// when fullscreen is not enabled initially. It will also
		/// contain the current position of a window after creation.
		/// </summary>
		glm::vec2 m_windowPosition = { 0.0f, 0.0f };

		/// <summary>
		/// This is used internally by the window to restore windows,
		/// because glfwRestoreWindow doesn't work as expected.
		/// It will not restore decorators.
		/// 
		/// Messing with this value will cause unexpected behavior.
		/// So, don't?
		/// 
		/// I guess you could set this on a specific
		/// window size change to change what size a window is
		/// restored to then next time it is restored. This should
		/// only be *used* when the restore button is clicked while
		/// a window is maximized or minimized. This value will be
		/// reset internally during any resizing operations.
		/// </summary>
		glm::vec2 m_previousWindowSize = { 0.0f, 0.0f };

		/// <summary>
		/// Same as the m_previousWindowSize
		/// </summary>
		glm::vec2 m_previousWindowPosition = { 0.0f, 0.0f };

		glm::vec2 m_DPIScale = { 1.0f, 1.0f };

		/// <summary>
		/// The window event message server.
		/// The window creation will populate this
		/// entry on creation if it is not already
		/// populated. 
		/// 
		/// Note: This is nullptr if it has not been
		/// created.
		/// </summary>
		OSEventMessenger* m_pMessenger = nullptr;

		/// <summary>
		/// Does the window have VSync enabled?
		/// </summary>
		bool m_isVSync = false;

		/// <summary>
		/// Is the window currently fullscreened
		/// </summary>
		bool m_isFullscreen = false;

		/// <summary>
		/// Is the window currently maximized.
		/// </summary>
		bool m_isMaximized = false;

		/// <summary>
		/// Does the window have decorators?
		/// </summary>
		bool m_hasDecorators = true;
	};
}