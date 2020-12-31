#pragma once

#include "Source/Window.h"

namespace sf
{
	class RenderWindow;
}

namespace Exelius
{
	class WindowsWindow
		: public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() final override;

		virtual unsigned int GetWidth() const final override { return m_data.m_width; }
		virtual unsigned int GetHeight() const final override { return m_data.m_height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) final override { m_data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) final override;
		virtual bool IsVSync() const final override;

		virtual void* GetNativeWindow() const { return m_pWindow; }

	private:
		virtual void Initialize(const WindowProperties& props);
		virtual void Shutdown();

	private:
		sf::RenderWindow* m_pWindow;

		struct WindowData
		{
			std::string m_title;
			unsigned int m_width;
			unsigned int m_height;
			bool m_isVSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};

}