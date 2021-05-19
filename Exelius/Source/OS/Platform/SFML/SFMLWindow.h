#pragma once
#include "Source/OS/Events/EventManagement.h"
#include "Source/Utility/Containers/Vector2.h"
#include "Source/Utility/Math/Rectangle.h"

#include "Source/OS/Platform/PlatformForwardDeclarations.h"

#include <EASTL/string.h>

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	/// <summary>
	/// Forward declaration of an SFML class.
	/// </summary>
	class RenderWindow;
	
	class Drawable;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(View);

	/// <summary>
	/// Window class that handles SFML specific function calls.
	/// The window also processes SFML events and passes them to
	/// the engines Event Management system.
	/// 
	/// TODO: SFML has some additional openGl functionality built
	///			into the windows. Try to utilize that more. See
	///			various "TODO"'s below.
	/// </summary>
	class SFMLWindow
	{
		sf::RenderWindow* m_pWindow;
		OSEventMessenger m_messenger;
		bool m_isVSync; // TODO: Elevate this to the Window interface.

	public:
		/// <summary>
		/// 
		/// </summary>
		SFMLWindow();

		/// <summary>
		/// The SFML window class that handles SFML specific events.
		/// This is the platform independent window class.
		/// </summary>
		/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
		/// <param name="width">The width of the window to be opened. Default: 1280</param>
		/// <param name="height">The height of the window to be opened. Default: 720</param>
		SFMLWindow(const eastl::string& title, const Vector2u& windowSize);
		SFMLWindow(const SFMLWindow&) = delete;
		SFMLWindow(SFMLWindow&&) = delete;
		SFMLWindow& operator=(const SFMLWindow&) = delete;
		SFMLWindow& operator=(SFMLWindow&&) = delete;
		~SFMLWindow();

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

		/// <summary>
		/// Check if the window is open.
		/// </summary>
		/// <returns>True if open, false if not.</returns>
		bool IsOpen() const;

		// TODO:
		//const ContextSettings& GetWindowSettings() const;

		/// <summary>
		/// Retrieves and notifies of any SFML events.
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
		void Draw(sf::Drawable* pDrawable);

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
		OSEventMessenger& GetEventMessenger() { return m_messenger; }

		/// <summary>
		/// Call any code required to shut down a window in SFML.
		/// </summary>
		void Shutdown();
	};
}