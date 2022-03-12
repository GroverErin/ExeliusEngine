#pragma once
#include "source/os/events/EventManagement.h"
#include "source/utility/containers/Vector2.h"
#include "source/utility/math/Rectangle.h"
#include "source/debug/Log.h"

#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/string.h>

struct GLFWwindow;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(View);
	FORWARD_DECLARE(Texture);
	FORWARD_DECLARE(VertexArray);

	FORWARD_DECLARE(RenderContext);

	/// <summary>
	/// Window class that handles OpenGL specific function calls.
	/// The window also processes OpenGL events and passes them to
	/// the engines Event Management system.
	/// </summary>
	class OpenGLWindow
	{
		struct WindowData
		{
			eastl::string m_title;
			Vector2u m_windowSize;
			bool m_isVSync = false;
			OSEventMessenger m_messenger;

			//EventCallFn EventCallback;
		};

		WindowData m_windowData;

		/// <summary>
		/// Log for the Graphics Interface.
		/// </summary>
		Log m_graphicsInterfaceLog;

		RenderContext* m_pRenderContext;

		GLFWwindow* m_pWindow;
		bool m_isVSync; // TODO: Elevate this to the Window interface.

	public:
		/// <summary>
		/// 
		/// </summary>
		OpenGLWindow();

		/// <summary>
		/// The OpenGL window class that handles OpenGL specific events.
		/// This is the platform independent window class.
		/// </summary>
		/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
		/// <param name="width">The width of the window to be opened. Default: 1280</param>
		/// <param name="height">The height of the window to be opened. Default: 720</param>
		OpenGLWindow(const eastl::string& title, const Vector2u& windowSize);
		OpenGLWindow(const OpenGLWindow&) = delete;
		OpenGLWindow(OpenGLWindow&&) = delete;
		OpenGLWindow& operator=(const OpenGLWindow&) = delete;
		OpenGLWindow& operator=(OpenGLWindow&&) = delete;
		~OpenGLWindow();

		/// <summary>
		/// Create (or recreate) the window.
		/// 
		/// TODO:
		///		Allow the passing of context variables,
		///		improved sf::VideoMode data,
		///		and WindowStyle.
		/// </summary>
		/// <param name="title"></param>
		/// <param name="windowSize"></param>
#undef CreateWindow // Defined in WinUser.h :(
		bool CreateWindow(const eastl::string& title, const Vector2u& windowSize);

		void InitializeRenderContext(Window* pAbstractWindow);

		/// <summary>
		/// Check if the window is open.
		/// </summary>
		/// <returns>True if open, false if not.</returns>
		bool IsOpen() const;

		// TODO:
		//const ContextSettings& GetWindowSettings() const;

		/// <summary>
		/// Retrieves and notifies of any OpenGL events.
		/// Calls pollEvent.
		/// </summary>
		void Update();

		// TODO: (for event polling on EventHandlingThread)
		//void WaitForEvent();

		// TODO: Need an Exelius::Image
		//sf::Image CaptureScreenshot() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Vector2i GetPosition() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="position"></param>
		void SetPosition(const Vector2i& position);

		/// <summary>
		/// Retrieves the current size of the window.
		/// </summary>
		/// <returns>Window size.</returns>
		Vector2u GetWindowSize() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowSize"></param>
		void SetWindowSize(const Vector2u& windowSize);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="title"></param>
		void SetWindowTitle(const eastl::string& title);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="size"></param>
		/// <param name="pPixels"></param>
		void SetIcon(const Vector2u& size, const uint8_t* pPixels);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="isVisible"></param>
		void SetVisible(bool isVisible);

		/// <summary>
		/// Sets VSync for this window.
		/// </summary>
		/// <param name="enabled">Whether to turn VSync on or off.</param>
		void SetVSync(bool isEnabled);

		/// <summary>
		/// Returns the VSync state of this window.
		/// </summary>
		/// <returns>Whether VSync is currently on or off.</returns>
		bool IsVSync() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="isVisible"></param>
		void SetMouseCursorVisible(bool isVisible);

		void SetMouseCursorGrabbed(bool isGrabbed);

		// TODO: (Define Cursor) Warning: Cursors are not supported on mobile.
		//void SetMouseCursor(const Cursor& cursor);

		void SetKeyRepeatEnabled(bool isEnabled);

		/// <summary>
		/// Probably don't use this? Will force renderthread to sleep and wake.
		/// See:
		/// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Window.php#af4322d315baf93405bf0d5087ad5e784
		/// </summary>
		/// <param name="framerateLimit"></param>
		void SetFramerateLimit(unsigned int framerateLimit);

		/// <summary>
		/// Change the joystick threshold.
		/// The joystick threshold is the value below which no JoystickMoved event will be generated.
		/// The threshold value is 0.1 by default.
		/// </summary>
		/// <param name="threshold"></param>
		void SetJoystickThreshold(float threshold);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="isActive"></param>
		/// <returns></returns>
		bool SetActive(bool isActive = true) const;

		/// <summary>
		/// 
		/// </summary>
		void RequestFocus();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool HasFocus() const;

		/// <summary>
		/// 
		/// </summary>
		void Render() const;

		// TODO:
		// This may need to change to not use sf::drawable?
		void Draw(const VertexArray& vertices, const Texture& texture);

		void Draw(const VertexArray& vertices);

		/// <summary>
		/// 
		/// </summary>
		void Clear();

		// TODO: May be necessary for OpenGL stuff.
		//WindowHandle GetOSWindowHandle() const;

		void SetView(const View& view);

		View GetView() const;

		View GetDefaultView() const;

		IRectangle GetViewport(const View& view) const;

		Vector2f MapPixelToCoords(const Vector2i& point) const;

		Vector2f MapPixelToCoords(const Vector2i& point, const View& view) const;

		Vector2i MapCoordsToPixel(const Vector2f& point) const;

		Vector2i MapCoordsToPixel(const Vector2f& point, const View& view) const;

		void PushGLStates();

		void PopGLStates();

		void ResetGLStates();

		/// <summary>
		/// Gets the event messenger owned by this window.
		/// </summary>
		/// <returns>This windows event messenger.</returns>
		OSEventMessenger& GetEventMessenger() { return m_windowData.m_messenger; }

		/// <summary>
		/// Call any code required to shut down a window in OpenGL.
		/// </summary>
		void Shutdown();

		GLFWwindow* GetGLFWWindow() const { EXE_ASSERT(m_pWindow); return m_pWindow; }
	};
}