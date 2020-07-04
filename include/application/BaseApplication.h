#pragma once

#include"Core.h"

#include"event/Event.h"
#include"event/EventQueue.h"
#include"event/InputCodes.h"

#include"utils/data/ArrayList.h"

#include"window/WindowSystem.h"

#include"state/StateMachine.h"

#include<functional>

INFINITY_TEMPLATE template class INFINITY_API std::function<void(Infinity::Event&)>;

namespace Infinity
{
	class INFINITY_API BaseApplication
	{
		using EventHandler = std::function<void(Event&)>;

	private:
		EventQueue m_event_queue;
		ArrayList<EventHandler> m_event_handlers;
		ArrayList<EventHandler> m_priority_event_handlers;

		Window::MainWindowParams m_main_params;
		WindowSystem m_window_system;

		State *m_start_state;

		static BaseApplication *s_application;

	public:
		BaseApplication(State *start_state);
		virtual ~BaseApplication();

		void AddEventHandler(EventHandler handler);
		void AddPriorityEventHandler(EventHandler handler);
		void PushEvent(Event *event);
		
		void Run();

		const WindowSystem &GetWindowSystem() const;

		static BaseApplication *GetApplication();

	private:
		void DispatchEvents();

		void EventHandlerFunc(Event &event);
	};
}