#include"InfinityPCH.h"

#include"application/BaseApplication.h"

#include"window/Window.h"
#include"window/BaseWindow.h"

#include"state/StateMachine.h"

#include"Context.h"
#include"Log.h"

#include"event/Event.h"

namespace Infinity
{
	BaseApplication *BaseApplication::s_application;

	BaseApplication::BaseApplication(State *start_state):
		m_event_queue(),
		m_event_handlers(),
		m_priority_event_handlers(),

		m_main_params(),
		m_window_system(),

		m_start_state(start_state)
	{
		s_application = this;
	}
	
	BaseApplication::~BaseApplication()
	{
		if (s_application == this) s_application = nullptr;
	}

	void BaseApplication::DispatchEvents()
	{
		Event *event;
		while (event = m_event_queue.PopEvent())
		{
			for (unsigned int i = 0; i < m_priority_event_handlers.GetSize(); ++i)
			{
				m_priority_event_handlers[i](*event);

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
				m_event_handlers[i](*event);

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

		AddEventHandler(INFINITY_TO_STATIC_EVENT_FUNC(BaseApplication::EventHandlerFunc));

		StateMachine state_machine(m_start_state);
		state_machine.Start();

		if (!m_window_system.InitMainWindow(Window::MainWindowParams()))
		{
			INFINITY_CORE_ERROR("Error initializing window system");
			return;
		}

		m_window_system.GetMainWindow()->MakeContextCurrent();

		DispatchEvents();

		m_window_system.GetMainWindow()->Show();

		INFINITY_CORE_TRACE("Application initialized");

		std::chrono::high_resolution_clock::time_point t1, t2;
		t1 = t2 = std::chrono::high_resolution_clock::now();

		while (!state_machine.ShouldExit())
		{
			t2 = std::chrono::high_resolution_clock::now();
			double dt = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
			t1 = t2;

			state_machine.PushEvents(dt);

			DispatchEvents();

			BaseWindow::PollInput();
			_Impl::Update();

			DispatchEvents();

			m_window_system.GetMainWindow()->SwapBuffers();
			for (Resource<Window> window : m_window_system.GetChildWindows())
			{
				if (ResourceCast<BaseWindow>(window)->AutoSwapBuffers()) window->SwapBuffers();
			}
		}

		INFINITY_CORE_TRACE("Application exited");
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
	
	void BaseApplication::EventHandlerFunc(Event &event)
	{
		switch (event.GetType())
		{
		case Event::EventType::WindowClosed:
		{
			WindowClosedEvent &wce = (WindowClosedEvent&)event;

			if (wce.GetWindow() == m_window_system.GetMainWindow())
			{
				PushEvent(new ApplicationInterruptedEvent);
			}

			break;
		}
		}
	}

	const WindowSystem &BaseApplication::GetWindowSystem() const { return m_window_system; }

	BaseApplication *BaseApplication::GetApplication() { return s_application; }
}