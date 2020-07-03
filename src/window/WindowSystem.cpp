#include"InfinityPCH.h"

#include"window/WindowSystem.h"
#include"window/BaseWindow.h"

#include"event/Event.h"

#include"application/BaseApplication.h"

namespace Infinity
{
	WindowSystem::WindowSystem():
		m_main_window(nullptr),
		m_child_windows()
	{}

	WindowSystem::~WindowSystem()
	{}

	void WindowSystem::Init()
	{
		BaseApplication::GetApplication()->AddEventHandler(INFINITY_TO_STATIC_EVENT_FUNC(WindowSystem::EventHandler));
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

		m_main_window = ResourceCast<Window>(BaseWindow::CreateBaseWindow());

		if (!ResourceCast<BaseWindow>(m_main_window)->Init(bw_params))
		{
			INFINITY_CORE_ERROR("Error initializing main window");
			return false;
		}

		return true;
	}

	Resource<Window> WindowSystem::GetMainWindow() const { return m_main_window; }
	const ArrayList<Resource<Window>> &WindowSystem::GetChildWindows() const { return m_child_windows; }

	Resource<Window> WindowSystem::CreateChildWindow(const Window::ChildWindowParams &params) const
	{
		BaseWindow::BaseWindowParams bw_params;
		bw_params.parent = ResourceCast<BaseWindow>(m_main_window);
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

		Resource<BaseWindow> base_window = BaseWindow::CreateBaseWindow();

		if (!base_window->Init(bw_params))
		{
			INFINITY_CORE_ERROR("Error initializing child window");
			return nullptr;
		}

		m_child_windows.Add(ResourceCast<Window>(base_window));

		return ResourceCast<Window>(base_window);
	}

	void WindowSystem::DestroyChildWindow(Resource<Window> window) const
	{
		if (Contains(m_child_windows, window))
			BaseApplication::GetApplication()->PushEvent(new WindowClosedEvent(window));
	}

	void WindowSystem::EventHandler(Event &event)
	{
		switch (event.GetType())
		{
		case Event::EventType::WindowClosed:
			Resource<Window> window = ((WindowClosedEvent&)event).GetWindow();

			if (window == GetMainWindow())
			{
				for (Resource<Window> window : GetChildWindows())
				{
					BaseApplication::GetApplication()->PushEvent(new WindowClosedEvent(window));
				}
			}
			else if (Contains(m_child_windows, window))
			{
				Remove(m_child_windows, window);
			}
			break;
		}
	}
}