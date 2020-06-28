#include"InfinityPCH.h"

#include"Application.h"

#include"window/Window.h"
#include"window/BaseWindow.h"
#include"Context.h"
#include"Log.h"

#include"event/Event.h"

namespace Infinity
{
	Application *Application::application = nullptr;

	Application::Application():
		m_event_queue(),
		m_event_listeners(),
		m_exit(false),

		m_main_params(),
		m_window_system()
	{
		application = this;
	}

	Application::~Application() {}

	void Application::DispatchEvents()
	{
		Event *event;
		while (event = m_event_queue.PopEvent())
		{
			for (unsigned int i = 0; i < m_event_listeners.GetSize(); ++i)
			{
				m_event_listeners[i](event);

				if (event->IsConsumed())
				{
					break;
				}
			}

			delete event;
		}
	}

	void Application::Run()
	{
		if (!BaseWindow::Init())
		{
			INFINITY_CORE_ERROR("Error initializing Window API");
			return;
		}

		WindowSystem::Init();

		AddEventListener(CallOnUserEvent);

		ApplicationEnteredEvent *entered_event = new ApplicationEnteredEvent(this);

		PushEvent(entered_event);
		DispatchEvents();

		if (!m_window_system.InitMainWindow(m_main_params))
		{
			INFINITY_CORE_ERROR("Error initializing window system");
			return;
		}

		m_window_system.GetMainWindow()->MakeContextCurrent();
		
		PushEvent(new UserCreateEvent(this));
		DispatchEvents();

		m_window_system.GetMainWindow()->Show();

		INFINITY_CORE_TRACE("Application initialized");

		std::chrono::high_resolution_clock::time_point t1, t2;
		t1 = t2 = std::chrono::high_resolution_clock::now();

		while (!m_exit)
		{
			t2 = std::chrono::high_resolution_clock::now();
			double dt = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
			t1 = t2;

			PushEvent(new UserUpdateEvent(dt, this));

			DispatchEvents();

			BaseWindow::PollInput();
			_Impl::Update();

			PushEvent(new UserRenderEvent(this));

			DispatchEvents();

			m_window_system.GetMainWindow()->SwapBuffers();
			for (Window *window : m_window_system.GetChildWindows())
			{
				if (((BaseWindow*)window)->AutoSwapBuffers()) window->SwapBuffers();
			}
		}

		INFINITY_CORE_TRACE("Application exited");

		PushEvent(new UserDestroyEvent(this));

		DispatchEvents();
	}

	void Application::AddEventListener(void(*listener)(Event*))
	{
		m_event_listeners.Add(listener);
	}

	void Application::PushEvent(Event *event) { m_event_queue.PushEvent(event); }
	void Application::RequestExit() { PushEvent(new ApplicationExitedEvent(nullptr)); }

	void Application::OnUserCreate(UserCreateEvent *event) {}
	void Application::OnUserUpdate(UserUpdateEvent *event) {}
	void Application::OnUserRender(UserRenderEvent *event) {}
	void Application::OnUserDestroy(UserDestroyEvent *event) {}

	void Application::OnApplicationEntered(ApplicationEnteredEvent *event) {}
	void Application::OnApplicationExited(ApplicationExitedEvent *event) {}

	void Application::OnWindowResized(WindowResizedEvent *event) {}
	void Application::OnWindowClosed(WindowClosedEvent *event) {}

	void Application::OnKeyPressed(KeyPressedEvent *event) {}
	void Application::OnKeyReleased(KeyReleasedEvent *event) {}
	void Application::OnMousePressed(MousePressedEvent *event) {}
	void Application::OnMouseReleased(MouseReleasedEvent *event) {}

	void Application::OnCursorEntered(CursorEnteredEvent *event) {}
	void Application::OnCursorExited(CursorExitedEvent *event) {}
	void Application::OnCursorMoved(CursorMovedEvent *event) {}

	void Application::CallOnUserEvent(Event *event)
	{
		switch (event->GetType())
		{
		case Event::EventType::UserCreate:
		{
			application->OnUserCreate((UserCreateEvent*)event);
			break;
		}
		case Event::EventType::UserUpdate:
		{
			application->OnUserUpdate((UserUpdateEvent*)event);
			break;
		}
		case Event::EventType::UserRender:
		{
			application->OnUserRender((UserRenderEvent*)event);
			break;
		}
		case Event::EventType::UserDestroy:
		{
			application->OnUserDestroy((UserDestroyEvent*)event);
			break;
		}
		case Event::EventType::ApplicationEntered:
		{
			ApplicationEnteredEvent *aee = (ApplicationEnteredEvent*)event;
			application->OnApplicationEntered(aee);
			application->m_main_params = aee->GetMainWindowParams();
			break;
		}
		case Event::EventType::ApplicationExited:
		{
			Window *main_window = application->m_window_system.GetMainWindow();
			
			if (!main_window->ShouldClose())
				application->PushEvent(new WindowClosedEvent(main_window));

			application->OnApplicationExited((ApplicationExitedEvent*)event);

			application->m_exit = true;
			break;
		}
		case Event::EventType::WindowResized:
		{
			application->OnWindowResized((WindowResizedEvent*)event);
			break;
		}
		case Event::EventType::WindowClosed:
		{
			if ((Window*)event->GetCaller() == application->m_window_system.GetMainWindow())
			{
				application->PushEvent(new ApplicationExitedEvent(application));
			}

			application->OnWindowClosed((WindowClosedEvent*)event);

			break;
		}
		case Event::EventType::KeyPressed:
		{
			application->OnKeyPressed((KeyPressedEvent*)event);
			break;
		}
		case Event::EventType::KeyReleased:
		{
			application->OnKeyReleased((KeyReleasedEvent*)event);
			break;
		}
		case Event::EventType::MousePressed:
		{
			application->OnMousePressed((MousePressedEvent*)event);
			break;
		}
		case Event::EventType::MouseReleased:
		{
			application->OnMouseReleased((MouseReleasedEvent*)event);
			break;
		}
		case Event::EventType::CursorEntered:
		{
			application->OnCursorEntered((CursorEnteredEvent*)event);
			break;
		}
		case Event::EventType::CursorExited:
		{
			application->OnCursorExited((CursorExitedEvent*)event);
			break;
		}
		case Event::EventType::CursorMoved:
		{
			application->OnCursorMoved((CursorMovedEvent*)event);
			break;
		}
		}
	}

	const WindowSystem &Application::GetWindowSystem() const { return m_window_system; }

	Application *Application::GetApplication() { return application; }
}