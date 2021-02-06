#pragma once
#include "Source/Events/Event.h"

namespace Exelius
{
	struct WindowProperties
	{
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;

		WindowProperties(const std::string& title = "Exelius Engine", unsigned int width = 1280, unsigned int height = 720)
			: m_title(title)
			, m_width(width)
			, m_height(height)
		{
			//
		}
	};

	// Interface representing a desktop system based window.
	// this should be implemented per platform.
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());
	};
}