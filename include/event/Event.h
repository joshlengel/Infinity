#pragma once

#include"Core.h"

#include"InputCodes.h"

#ifdef DEBUG
	#include<string>
#endif // DEBUG

namespace Infinity
{
	class Window;
	class Context;

	class INFINITY_API Event
	{
	public:
		enum class EventType : unsigned char
		{
			WindowResized, WindowClosed, ApplicationExited,
			UserCreate, UserUpdate, UserRender, UserDestroy,
			KeyPressed, KeyReleased, MousePressed, MouseReleased,
			CursorEntered, CursorExited, CursorMoved
		};

	private:
		EventType m_type;
		bool m_consumed;

#ifdef DEBUG
		std::string m_debug_name;
#endif // DEBUG

		void *m_caller;

	public:
#ifdef DEBUG
		Event(EventType type, const std::string &debug_name, void *caller);
		Event(EventType type, std::string &&debug_name, void *caller);

		const std::string &GetDebugName() const;
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
		unsigned int m_width, m_height;

	public:
		WindowResizedEvent(unsigned int width, unsigned int height, void *caller);
		~WindowResizedEvent();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	};

	class INFINITY_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(void *caller);
		~WindowClosedEvent();
	};

	// Application events

	class INFINITY_API ApplicationExitedEvent : public Event
	{
	public:
		ApplicationExitedEvent(void *caller);
		~ApplicationExitedEvent();
	};

	// User events

	class INFINITY_API UserCreateEvent : public Event
	{
	private:
		Window *m_window;
		Context *m_context;

	public:
		UserCreateEvent(Window *window, Context *context, void *caller);
		~UserCreateEvent();

		Window *GetWindow() const;
		Context *GetContext() const;
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
	private:
		Context *m_context;

	public:
		UserRenderEvent(Context *context, void *caller);
		~UserRenderEvent();

		Context *GetContext() const;
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