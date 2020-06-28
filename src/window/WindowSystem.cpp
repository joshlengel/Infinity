#include"InfinityPCH.h"

#include"window/WindowSystem.h"
#include"window/BaseWindow.h"

#include"event/Event.h"

#include"Application.h"

namespace Infinity
{
	void WindowSystem::Init()
	{
		Application::GetApplication()->AddEventListener(EventHandler);
	}

	WindowSystem::WindowSystem():
		m_main_window(nullptr),
		m_child_windows()
	{}

	WindowSystem::~WindowSystem()
	{
		((BaseWindow*)m_main_window)->Destroy();
		delete m_main_window;

		for (Window *window : m_child_windows)
		{
			((BaseWindow*)window)->Destroy();
			delete window;
		}
	}

	bool WindowSystem::InitMainWindow(const Window::MainWindowParams &params)
	{
		BaseWindow::BaseWindowParams bw_params;
		bw_params.parent = nullptr;
		bw_params.style = BaseWindow::BaseWindowStyle::MainWindow;
		bw_params.title = params.title;
		bw_params.x = params.x;
		bw_params.y = params.y;
		bw_params.width = params.width;
		bw_params.height = params.height;
		bw_params.fullscreen = params.fullscreen;
		bw_params.vsync = params.vsync;
		bw_params.auto_show = params.auto_show;
		bw_params.auto_swap_buffers = params.auto_swap_buffers;
		bw_params.enable_alt_enter_fullscreen = params.enable_alt_enter_fullscreen;
		bw_params.icon = params.icon;

		m_main_window = BaseWindow::CreateBaseWindow();

		if (!((BaseWindow*)m_main_window)->Init(bw_params))
		{
			INFINITY_CORE_ERROR("Error initializing main window");
			return false;
		}

		return true;
	}

	Window *WindowSystem::GetMainWindow() const { return m_main_window; }
	const ArrayList<Window*> &WindowSystem::GetChildWindows() const { return m_child_windows; }

	Window *WindowSystem::CreateChildWindow(const Window::ChildWindowParams &params) const
	{
		BaseWindow::BaseWindowParams bw_params;
		bw_params.parent = (BaseWindow*)m_main_window;
		bw_params.style = BaseWindow::BaseWindowStyle::ChildWindow;
		bw_params.title = params.title;
		bw_params.x = params.x;
		bw_params.y = params.y;
		bw_params.width = params.width;
		bw_params.height = params.height;
		bw_params.fullscreen = false;
		bw_params.vsync = params.vsync;
		bw_params.auto_show = false;
		bw_params.auto_swap_buffers = params.auto_swap_buffers;
		bw_params.enable_alt_enter_fullscreen = false;
		bw_params.icon = params.icon;

		BaseWindow *base_window = BaseWindow::CreateBaseWindow();

		if (!base_window->Init(bw_params))
		{
			INFINITY_CORE_ERROR("Error initializing child window");
			delete base_window;
			return nullptr;
		}

		m_child_windows.Add(base_window);

		return base_window;
	}

	void WindowSystem::DestroyChildWindow(Window *window) const
	{
		if (window)
		{
			((BaseWindow*)window)->Destroy();
			delete window;

			m_child_windows.Remove(window);
		}
	}

	void WindowSystem::EventHandler(Event *event)
	{
		switch (event->GetType())
		{
		case Event::EventType::WindowClosed:
			const WindowSystem &system = Application::GetApplication()->GetWindowSystem();
			BaseWindow *window = (BaseWindow*)event->GetCaller();

			if (window == system.GetMainWindow())
			{
				for (Window *window : system.GetChildWindows())
				{
					Application::GetApplication()->PushEvent(new WindowClosedEvent(window));
				}
			}
			else if (system.GetChildWindows().Contains(window))
			{
				system.m_child_windows.Remove(window);
				window->Destroy();
				delete window;
			}
			break;
		}
	}
}