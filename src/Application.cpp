#include"Application.h"

#include"Window.h"
#include"Context.h"
#include"Log.h"

#include"event/Event.h"

#include<chrono>

namespace Infinity
{
	Application *Application::application = nullptr;

	Application::Application():
		m_window(nullptr),
		m_context(nullptr),
		m_queue(),
		m_event_listeners()
	{
		application = this;
	}

	Application::~Application() {}

	void Application::DispatchEvents()
	{
		Event *event;
		while (event = m_queue.PopEvent())
		{
			for (unsigned int i = 0; i < m_event_listeners.size(); ++i)
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
		Window::InitListeners();

		AddEventListener(CallOnUserEvent);

		m_window = Window::CreateWindow();
		m_context = Context::CreateContext();

		WindowParams params;

		std::chrono::high_resolution_clock::time_point t1, t2;

		if (!m_window->Init(params))
		{
			INFINITY_CORE_ERROR("Error initializing window");
			goto clean_up;
		}
		else
		{
			INFINITY_CORE_TRACE("Window created successfully");
		}

		m_window->MakeContextCurrent();

		if (!m_context->Init())
		{
			INFINITY_CORE_ERROR("Error initializing context");
			goto clean_up;
		}
		else
		{
			INFINITY_CORE_TRACE("Context created successfully");
		}

		PushEvent(new UserCreateEvent(m_context, this));

		DispatchEvents();

		m_window->Show();

		t1 = t2 = std::chrono::high_resolution_clock::now();

		while (!m_window->ShouldClose())
		{
			t2 = std::chrono::high_resolution_clock::now();
			double dt = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
			t1 = t2;

			Window::PollInput();

			PushEvent(new UserUpdateEvent(dt, this));
			PushEvent(new UserRenderEvent(m_context, this));

			DispatchEvents();

			m_window->SwapBuffers();
		}

		INFINITY_CORE_TRACE("Window closed");

		PushEvent(new UserDestroyEvent(this));

		DispatchEvents();

	clean_up:
		m_context->Destroy();
		m_window->Destroy();

		delete m_context;
		delete m_window;
	}

	void Application::AddEventListener(void(*listener)(Event*))
	{
		m_event_listeners.push_back(listener);
	}

	void Application::PushEvent(Event *event) { m_queue.PushEvent(event); }
	void Application::RequestExit() { PushEvent(new ApplicationExitedEvent(nullptr)); }

	void Application::OnUserCreate(UserCreateEvent *event) {}
	void Application::OnUserUpdate(UserUpdateEvent *event) {}
	void Application::OnUserRender(UserRenderEvent *event) {}
	void Application::OnUserDestroy(UserDestroyEvent *event) {}

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
		case Event::EventType::ApplicationExited:
		{
			application->PushEvent(new WindowClosedEvent(application->m_window));
			application->OnApplicationExited((ApplicationExitedEvent*)event);
			break;
		}
		case Event::EventType::WindowResized:
		{
			application->OnWindowResized((WindowResizedEvent*)event);
			break;
		}
		case Event::EventType::WindowClosed:
		{
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

	bool Application::KeyDown(KeyCode key) { return m_window->KeyDown(key); }
	bool Application::KeyPressed(KeyCode key) { return m_window->KeyPressed(key); }
	bool Application::KeyReleased(KeyCode key) { return m_window->KeyReleased(key); }
	bool Application::MouseDown(MouseCode button) { return m_window->MouseDown(button); }
	bool Application::MousePressed(MouseCode button) { return m_window->MousePressed(button); }
	bool Application::MouseReleased(MouseCode button) { return m_window->MouseReleased(button); }

	Application *Application::GetApplication() { return application; }
}