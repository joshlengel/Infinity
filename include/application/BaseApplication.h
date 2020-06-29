#pragma once

#include"Core.h"

#include"event/EventQueue.h"
#include"event/InputCodes.h"

#include"utils/data/ArrayList.h"

#include"window/WindowSystem.h"

#include<functional>

namespace Infinity
{
	class Event;
	class Application;
}

typedef std::function<void(Infinity::Event*)> EventHandler;

INFINITY_TEMPLATE template class INFINITY_API std::function<void(Infinity::Event*)>;

namespace Infinity
{
	INFINITY_TEMPLATE template class INFINITY_API ArrayList<EventHandler>;

	class INFINITY_API BaseApplication
	{
	private:
		EventQueue m_event_queue;
		ArrayList<EventHandler> m_event_handlers;
		ArrayList<EventHandler> m_priority_event_handlers;

		Window::MainWindowParams m_main_params;
		WindowSystem m_window_system;

		bool m_exit;

		static BaseApplication *s_application;

		Application *m_application;

	public:
		BaseApplication(Application *application);
		virtual ~BaseApplication();

		void AddEventHandler(EventHandler handler);
		void AddPriorityEventHandler(EventHandler handler);
		void PushEvent(Event *event);
		void RequestExit();

		void Run();

		const WindowSystem &GetWindowSystem() const;

		static BaseApplication *GetApplication();

	private:
		void DispatchEvents();

		void CallOnUserEvent(Event *event);
	};
}