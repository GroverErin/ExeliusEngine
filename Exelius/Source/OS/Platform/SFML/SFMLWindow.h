#pragma once

#include <string>
#include "Source/OS/Events/EventManagement.h"

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	/// <summary>
	/// Forward declaration of an SFML class.
	/// </summary>
	class RenderWindow;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Window class that handles SFML specific function calls.
	/// The window also processes SFML events and passes them to
	/// the engines Event Management system.
	/// </summary>
	class SFMLWindow
	{
	public:
		/// <summary>
		/// The SFML window class that handles SFML specific events.
		/// This is the platform independent window class.
		/// </summary>
		/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
		/// <param name="width">The width of the window to be opened. Default: 1280</param>
		/// <param name="height">The height of the window to be opened. Default: 720</param>
		SFMLWindow(const std::string& title, unsigned int width, unsigned int height);
		~SFMLWindow();

		/// <summary>
		/// Retrieves and notifies of any SFML events.
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// Retrieves the current width of the window.
		/// </summary>
		/// <returns>Window width.</returns>
		unsigned int GetWidth() const { return m_width; }

		/// <summary>
		/// Retrieves the current height of the window.
		/// </summary>
		/// <returns>Window height.</returns>
		unsigned int GetHeight() const { return m_height; }

		/// <summary>
		/// Sets VSync for this window.
		/// </summary>
		/// <param name="enabled">Whether to turn VSync on or off.</param>
		void SetVSync(bool enabled);

		/// <summary>
		/// Returns the VSync state of this window.
		/// </summary>
		/// <returns>Whether VSync is currently on or off.</returns>
		bool IsVSync() const;

		/// <summary>
		/// Gets the event messenger owned by this window.
		/// </summary>
		/// <returns>This windows event messenger.</returns>
		OSEventMessenger& GetEventMessenger() { return m_messenger; }

	private:
		/// <summary>
		/// Call any SFML initialization code.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Call any code required to shut down a window in SFML.
		/// </summary>
		void Shutdown();

		sf::RenderWindow* m_pWindow;
		OSEventMessenger m_messenger;

		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool m_isVSync;
	};
}