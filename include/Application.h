#pragma once

#include"Core.h"

#include"event/EventQueue.h"
#include"event/InputCodes.h"

#include"utils/data/ArrayList.h"

namespace Infinity
{
	class Window;
	class Context;

	class Event;
	class WindowResizedEvent;
	class WindowClosedEvent;
	class ApplicationExitedEvent;
	class UserCreateEvent;
	class UserUpdateEvent;
	class UserRenderEvent;
	class UserDestroyEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MousePressedEvent;
	class MouseReleasedEvent;
	class CursorEnteredEvent;
	class CursorExitedEvent;
	class CursorMovedEvent;

	INFINITY_TEMPLATE template class INFINITY_API ArrayList<void(*)(Event*)>;

	class INFINITY_API Application
	{
	private:
		Window *m_window;
		Context *m_context;
		EventQueue m_queue;

		ArrayList<void(*)(Event*)> m_event_listeners;

		static Application *application;

	public:
		Application();
		virtual ~Application();

		void AddEventListener(void(*listener)(Event*));
		void PushEvent(Event *event);
		void RequestExit();

		void Run();

		virtual void OnUserCreate(UserCreateEvent *event);
		virtual void OnUserUpdate(UserUpdateEvent *event);
		virtual void OnUserRender(UserRenderEvent *event);
		virtual void OnUserDestroy(UserDestroyEvent *event);

		virtual void OnApplicationExited(ApplicationExitedEvent *event);

		virtual void OnWindowResized(WindowResizedEvent *event);
		virtual void OnWindowClosed(WindowClosedEvent *event);

		virtual void OnKeyPressed(KeyPressedEvent *event);
		virtual void OnKeyReleased(KeyReleasedEvent *event);
		virtual void OnMousePressed(MousePressedEvent *event);
		virtual void OnMouseReleased(MouseReleasedEvent *event);

		virtual void OnCursorEntered(CursorEnteredEvent *event);
		virtual void OnCursorExited(CursorExitedEvent *event);
		virtual void OnCursorMoved(CursorMovedEvent *event);

		bool KeyDown(KeyCode key);
		bool KeyPressed(KeyCode key);
		bool KeyReleased(KeyCode key);
		bool MouseDown(MouseCode button);
		bool MousePressed(MouseCode button);
		bool MouseReleased(MouseCode button);

		static Application *GetApplication();

	private:
		void DispatchEvents();

		static void CallOnUserEvent(Event *event);
	};

	extern Application *CreateApplication();
}