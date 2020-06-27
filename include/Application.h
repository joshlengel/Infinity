#pragma once

#include"Core.h"

#include"event/EventQueue.h"
#include"event/InputCodes.h"

#include"utils/data/ArrayList.h"

#include"Window.h"

namespace Infinity
{
	class Event;
	class WindowResizedEvent;
	class WindowClosedEvent;
	class ApplicationEnteredEvent;
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
		EventQueue m_event_queue;
		ArrayList<void(*)(Event*)> m_event_listeners;

		Window *m_main_window;
		Window::WindowParams m_main_params;
		ArrayList<Window*> m_windows;

		bool m_exit;

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

		virtual void OnApplicationEntered(ApplicationEnteredEvent *event);
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

		bool KeyDown(KeyCode key) const;
		bool KeyPressed(KeyCode key) const;
		bool KeyReleased(KeyCode key) const;
		bool MouseDown(MouseCode key) const;
		bool MousePressed(MouseCode key) const;
		bool MouseReleased(MouseCode key) const;

		Window *GetMainWindow();

		static Application *GetApplication();

		void AddWindow(Window *window);
		void RemoveWindow(Window *window);
		const ArrayList<Window*> &GetWindows() const;

	private:
		void DispatchEvents();

		static void CallOnUserEvent(Event *event);
	};

	extern Application *CreateApplication();
}