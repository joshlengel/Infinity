#pragma once

#include"Core.h"

#include"InputCodes.h"

#ifdef DEBUG
#include"utils/data/String.h"
#endif // DEBUG

#include"Window.h"

namespace Infinity
{
	class INFINITY_API Event
	{
	public:
		enum class EventType : unsigned char
		{
			WindowResized, WindowClosed,
			ApplicationEntered, ApplicationExited,
			UserCreate, UserUpdate, UserRender, UserDestroy,
			KeyPressed, KeyReleased, MousePressed, MouseReleased,
			CursorEntered, CursorExited, CursorMoved
		};

	private:
		EventType m_type;
		bool m_consumed;

#ifdef DEBUG
		String m_debug_name;
#endif // DEBUG

		void *m_caller;

	public:
#ifdef DEBUG
		Event(EventType type, const String &debug_name, void *caller);
		Event(EventType type, String &&debug_name, void *caller);

		const String &GetDebugName() const;
#else
		Event(EventType type, void *caller);
#endif // DEBUG
		virtual ~Event();

		EventType GetType() const;
		void Consume();
		bool IsConsumed() const;

		void *GetCaller() const;
	};

	// Window events

	class INFINITY_API WindowResizedEvent : public Event
	{
	private:
		Window *m_window;
		unsigned int m_width, m_height;

	public:
		WindowResizedEvent(Window *window, unsigned int width, unsigned int height, void *caller);
		~WindowResizedEvent();

		Window *GetWindow() const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	};

	class INFINITY_API WindowClosedEvent : public Event
	{
	private:
		Window *m_window;

	public:
		WindowClosedEvent(Window *window, void *caller);
		~WindowClosedEvent();

		Window *GetWindow() const;
	};

	// Application events

	class INFINITY_API ApplicationEnteredEvent : public Event
	{
	private:
		Window::WindowParams m_params;

	public:
		ApplicationEnteredEvent(void *caller);
		~ApplicationEnteredEvent();

		Window::WindowParams &GetMainWindowParams();
	};

	class INFINITY_API ApplicationExitedEvent : public Event
	{
	public:
		ApplicationExitedEvent(void *caller);
		~ApplicationExitedEvent();
	};

	// User events

	class INFINITY_API UserCreateEvent : public Event
	{
	public:
		UserCreateEvent(void *caller);
		~UserCreateEvent();
	};

	class INFINITY_API UserUpdateEvent : public Event
	{
	private:
		double m_dt;

	public:
		UserUpdateEvent(double dt, void *caller);
		~UserUpdateEvent();

		double GetDT() const;
	};

	class INFINITY_API UserRenderEvent : public Event
	{
	public:
		UserRenderEvent(void *caller);
		~UserRenderEvent();
	};

	class INFINITY_API UserDestroyEvent : public Event
	{
	public:
		UserDestroyEvent(void *caller);
		~UserDestroyEvent();
	};

	// keyboard events

	class INFINITY_API KeyPressedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyPressedEvent(KeyCode key, void *caller);
		~KeyPressedEvent();

		KeyCode GetKey() const;
	};

	class INFINITY_API KeyReleasedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyReleasedEvent(KeyCode key, void *caller);
		~KeyReleasedEvent();

		KeyCode GetKey() const;
	};

	// mouse events

	class INFINITY_API MousePressedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MousePressedEvent(MouseCode button, void *caller);
		~MousePressedEvent();

		MouseCode GetButton() const;
	};

	class INFINITY_API MouseReleasedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MouseReleasedEvent(MouseCode button, void *caller);
		~MouseReleasedEvent();

		MouseCode GetButton() const;
	};

	// cursor events

	class INFINITY_API CursorEnteredEvent : public Event
	{
	public:
		CursorEnteredEvent(void *caller);
		~CursorEnteredEvent();
	};

	class INFINITY_API CursorExitedEvent : public Event
	{
	public:
		CursorExitedEvent(void *caller);
		~CursorExitedEvent();
	};

	class INFINITY_API CursorMovedEvent : public Event
	{
	private:
		int m_cx, m_cy;

	public:
		CursorMovedEvent(int cx, int cy, void *caller);
		~CursorMovedEvent();

		int GetCX() const;
		int GetCY() const;
	};
}