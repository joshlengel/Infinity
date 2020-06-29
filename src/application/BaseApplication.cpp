#include"InfinityPCH.h"

#include"application/BaseApplication.h"

#include"window/Window.h"
#include"window/BaseWindow.h"
#include"Context.h"
#include"Log.h"

#include"event/Event.h"

#include"application/Application.h"

namespace Infinity
{
	BaseApplication *BaseApplication::s_application = nullptr;

	BaseApplication::BaseApplication(Application *application):
		m_event_queue(),
		m_event_handlers(),
		m_priority_event_handlers(),

		m_main_params(),
		m_window_system(),

		m_exit(false),
		m_application(application)
	{
		s_application = this;
	}
	
	BaseApplication::~BaseApplication()
	{
		if (s_application->m_application == m_application) s_application = nullptr;
		delete m_application;
	}

	void BaseApplication::DispatchEvents()
	{
		Event *event;
		while (event = m_event_queue.PopEvent())
		{
			for (unsigned int i = 0; i < m_priority_event_handlers.GetSize(); ++i)
			{
				m_priority_event_handlers[i](event);

				if (event->IsConsumed())
				{
					break;
				}
			}

			if (event->IsConsumed())
			{
				delete event;
				continue;
			}

			for (unsigned int i = 0; i < m_event_handlers.GetSize(); ++i)
			{
				m_event_handlers[i](event);

				if (event->IsConsumed())
				{
					break;
				}
			}

			delete event;
		}
	}

	void BaseApplication::Run()
	{
		if (!BaseWindow::Init())
		{
			INFINITY_CORE_ERROR("Error initializing Window API");
			return;
		}

		ApplicationEnteredEvent *entered_event = new ApplicationEnteredEvent(this);
		m_application->OnApplicationEntered(entered_event);
		m_main_params = entered_event->GetMainWindowParams();
		delete entered_event;
		
		if (!m_window_system.InitMainWindow(m_main_params))
		{
			INFINITY_CORE_ERROR("Error initializing window system");
			return;
		}

		AddEventHandler(INFINITY_TO_STATIC_EVENT_FUNC(BaseApplication::CallOnUserEvent));

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

	void BaseApplication::AddEventHandler(EventHandler handler)
	{
		m_event_handlers.Add(handler);
	}

	void BaseApplication::AddPriorityEventHandler(EventHandler handler)
	{
		m_priority_event_handlers.Add(handler);
	}

	void BaseApplication::PushEvent(Event *event) { m_event_queue.PushEvent(event); }
	void BaseApplication::RequestExit() { PushEvent(new ApplicationExitedEvent(nullptr)); }

	void BaseApplication::CallOnUserEvent(Event *event)
	{
		switch (event->GetType())
		{
		case Event::EventType::UserCreate:
		{
			m_application->OnUserCreate((UserCreateEvent*)event);
			break;
		}
		case Event::EventType::UserUpdate:
		{
			m_application->OnUserUpdate((UserUpdateEvent*)event);
			break;
		}
		case Event::EventType::UserRender:
		{
			m_application->OnUserRender((UserRenderEvent*)event);
			break;
		}
		case Event::EventType::UserDestroy:
		{
			m_application->OnUserDestroy((UserDestroyEvent*)event);
			break;
		}
		case Event::EventType::ApplicationExited:
		{
			Window *main_window = m_window_system.GetMainWindow();

			if (!main_window->ShouldClose())
				PushEvent(new WindowClosedEvent(main_window));

			m_application->OnApplicationExited((ApplicationExitedEvent*)event);

			m_exit = true;
			break;
		}
		case Event::EventType::WindowResized:
		{
			m_application->OnWindowResized((WindowResizedEvent*)event);
			break;
		}
		case Event::EventType::WindowClosed:
		{
			if ((Window*)event->GetCaller() == m_window_system.GetMainWindow())
			{
				PushEvent(new ApplicationExitedEvent(this));
			}

			m_application->OnWindowClosed((WindowClosedEvent*)event);

			break;
		}
		case Event::EventType::KeyPressed:
		{
			m_application->OnKeyPressed((KeyPressedEvent*)event);
			break;
		}
		case Event::EventType::KeyReleased:
		{
			m_application->OnKeyReleased((KeyReleasedEvent*)event);
			break;
		}
		case Event::EventType::MousePressed:
		{
			m_application->OnMousePressed((MousePressedEvent*)event);
			break;
		}
		case Event::EventType::MouseReleased:
		{
			m_application->OnMouseReleased((MouseReleasedEvent*)event);
			break;
		}
		case Event::EventType::CursorEntered:
		{
			m_application->OnCursorEntered((CursorEnteredEvent*)event);
			break;
		}
		case Event::EventType::CursorExited:
		{
			m_application->OnCursorExited((CursorExitedEvent*)event);
			break;
		}
		case Event::EventType::CursorMoved:
		{
			m_application->OnCursorMoved((CursorMovedEvent*)event);
			break;
		}
		}
	}

	const WindowSystem &BaseApplication::GetWindowSystem() const { return m_window_system; }

	BaseApplication *BaseApplication::GetApplication() { return s_application; }
}