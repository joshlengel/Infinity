#include"InfinityPCH.h"

#include"Application.h"

#include"Window.h"
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

		m_main_window(nullptr),
		m_main_params(),
		m_windows()
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
		if (!Window::Init())
		{
			INFINITY_CORE_ERROR("Error initializing Window API");
			return;
		}

		AddEventListener(CallOnUserEvent);

		ApplicationEnteredEvent *entered_event = new ApplicationEnteredEvent(this);

		PushEvent(entered_event);
		DispatchEvents();

		m_main_window = Window::CreateWindow();

		if (!m_main_window->Init(m_main_params))
		{
			INFINITY_CORE_ERROR("Error creating main window");
			return;
		}

		m_main_window->MakeContextCurrent();
		
		PushEvent(new UserCreateEvent(this));
		DispatchEvents();

		if (m_main_params.auto_show)
			m_main_window->Show();

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

			Window::PollInput();

			PushEvent(new UserRenderEvent(this));

			DispatchEvents();

			for (Window *window : m_windows)
			{
				if (window->AutoSwapBuffers()) window->SwapBuffers();
			}
		}

		INFINITY_CORE_TRACE("Application exited");

		PushEvent(new UserDestroyEvent(this));

		DispatchEvents();

		if (m_main_window)
		{
			m_main_window->Destroy();
			delete m_main_window;
		}
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

	bool Application::KeyDown(KeyCode key) const { return m_main_window->KeyDown(key); }
	bool Application::KeyPressed(KeyCode key) const { return m_main_window->KeyPressed(key); }
	bool Application::KeyReleased(KeyCode key) const { return m_main_window->KeyReleased(key); }
	bool Application::MouseDown(MouseCode key) const { return m_main_window->MouseDown(key); }
	bool Application::MousePressed(MouseCode key) const { return m_main_window->MousePressed(key); }
	bool Application::MouseReleased(MouseCode key) const { return m_main_window->MouseReleased(key); }

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
			application->OnApplicationExited((ApplicationExitedEvent*)event);

			for (Window *window : application->m_windows)
			{
				application->PushEvent(new WindowClosedEvent(window, window));
			}

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
			WindowClosedEvent *wce = (WindowClosedEvent*)event;
			application->OnWindowClosed(wce);

			if (application->m_windows.Empty() || wce->GetWindow() == application->m_main_window)
			{
				application->PushEvent(new ApplicationExitedEvent(application));
			}

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

	Window *Application::GetMainWindow() { return m_main_window; }

	Application *Application::GetApplication() { return application; }

	void Application::AddWindow(Window *window) { m_windows.Add(window); }
	void Application::RemoveWindow(Window *window) { m_windows.Remove(window); }
	const ArrayList<Window*> &Application::GetWindows() const { return m_windows; }
}