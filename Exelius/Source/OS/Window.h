#pragma once
#include "Source/OS/Events/EventManagement.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CCRT.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplWindow>
	class _Window
	{
	public:
		/// <summary>
		/// The purpose of this constructor is to pass the window parameters to the Implementation Window.
		/// </summary>
		/// <param name="title">The name of the window to be opened."</param>
		/// <param name="width">The width of the window to be opened.</param>
		/// <param name="height">The height of the window to be opened.</param>
		_Window(const std::string& title, unsigned int width, unsigned int height)
			: m_impl(title, width, height)
		{
		}

		/// <summary>
		/// Calls the implementations update method.
		/// </summary>
		void OnUpdate() { m_impl.OnUpdate(); }

		/// <summary>
		/// Calls the implementations get width method.
		/// </summary>
		unsigned int GetWidth() const { m_impl.GetWidth(); }

		/// <summary>
		/// Calls the implementations get height method.
		/// </summary>
		unsigned int GetHeight() const { m_impl.GetHeight(); }

		/// <summary>
		/// Calls the implementations set VSync method.
		/// </summary>
		/// <param name="enabled">Set VSync on or off.</param>
		void SetVSync(bool enabled) const { m_impl.SetVSync(enabled); }

		/// <summary>
		/// Calls the implementations Is VSync method.
		/// </summary>
		/// <returns></returns>
		bool IsVSync() const { return m_impl.IsVSync(); }

		/// <summary>
		/// Gets the implmentations OS Event messenger.
		/// </summary>
		/// <returns>The implementations OS Event messenger.</returns>
		OSEventMessenger& GetEventMessenger() { return m_impl.GetEventMessenger(); }
	private:
		ImplWindow m_impl;
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "Platform/SFML/SFMLWindow.h"
	namespace Exelius
	{
		using Window = _Window<SFMLWindow>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif