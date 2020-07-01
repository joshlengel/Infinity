#pragma once

#include"Core.h"

#include"InputCodes.h"

#ifdef DEBUG
#include"utils/data/String.h"
#endif // DEBUG

#include"utils/data/Resource.h"

#include"window/Window.h"

namespace Infinity
{
	class INFINITY_API Event
	{
	public:
		enum class EventType : unsigned char
		{
			// Handleable by client
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

	public:
#ifdef DEBUG
		Event(EventType type, const String &debug_name);
		Event(EventType type, String &&debug_name);

		const String &GetDebugName() const;
#else
		Event(EventType type);
#endif // DEBUG
		virtual ~Event();

		EventType GetType() const;
		void Consume();
		bool IsConsumed() const;
	};

	// Window events

	class INFINITY_API WindowResizedEvent : public Event
	{
	private:
		Resource<Window> m_window;
		unsigned int m_width, m_height;

	public:
		WindowResizedEvent(unsigned int width, unsigned int height, Resource<Window> window);
		~WindowResizedEvent();

		Resource<Window> GetWindow() const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	};

	class INFINITY_API WindowClosedEvent : public Event
	{
	private:
		Resource<Window> m_window;

	public:
		WindowClosedEvent(Resource<Window> window);

		Resource<Window> GetWindow() const;
		~WindowClosedEvent();
	};

	// Application events

	class INFINITY_API ApplicationEnteredEvent : public Event
	{
	private:
		Window::MainWindowParams m_params;

	public:
		ApplicationEnteredEvent();
		~ApplicationEnteredEvent();

		Window::MainWindowParams &GetMainWindowParams();
	};

	class INFINITY_API ApplicationExitedEvent : public Event
	{
	public:
		ApplicationExitedEvent();
		~ApplicationExitedEvent();
	};

	// User events

	class INFINITY_API UserCreateEvent : public Event
	{
	public:
		UserCreateEvent();
		~UserCreateEvent();
	};

	class INFINITY_API UserUpdateEvent : public Event
	{
	private:
		double m_dt;

	public:
		UserUpdateEvent(double dt);
		~UserUpdateEvent();

		double GetDT() const;
	};

	class INFINITY_API UserRenderEvent : public Event
	{
	public:
		UserRenderEvent();
		~UserRenderEvent();
	};

	class INFINITY_API UserDestroyEvent : public Event
	{
	public:
		UserDestroyEvent();
		~UserDestroyEvent();
	};

	// keyboard events

	class INFINITY_API KeyPressedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyPressedEvent(KeyCode key);
		~KeyPressedEvent();

		KeyCode GetKey() const;
	};

	class INFINITY_API KeyReleasedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyReleasedEvent(KeyCode key);
		~KeyReleasedEvent();

		KeyCode GetKey() const;
	};

	// mouse events

	class INFINITY_API MousePressedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MousePressedEvent(MouseCode button);
		~MousePressedEvent();

		MouseCode GetButton() const;
	};

	class INFINITY_API MouseReleasedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MouseReleasedEvent(MouseCode button);
		~MouseReleasedEvent();

		MouseCode GetButton() const;
	};

	// cursor events

	class INFINITY_API CursorEnteredEvent : public Event
	{
	private:
		Resource<Window> m_window;

	public:
		CursorEnteredEvent(Resource<Window> window);
		~CursorEnteredEvent();

		Resource<Window> GetWindow() const;
	};

	class INFINITY_API CursorExitedEvent : public Event
	{
	private:
		Resource<Window> m_window;

	public:
		CursorExitedEvent(Resource<Window> window);
		~CursorExitedEvent();

		Resource<Window> GetWindow() const;
	};

	class INFINITY_API CursorMovedEvent : public Event
	{
	private:
		Resource<Window> m_window;

		int m_cx, m_cy;

	public:
		CursorMovedEvent(int cx, int cy, Resource<Window> window);
		~CursorMovedEvent();

		Resource<Window> GetWindow() const;

		int GetCX() const;
		int GetCY() const;
	};
}