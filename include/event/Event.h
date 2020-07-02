#pragma once

#include"Core.h"

#include"InputCodes.h"

#include"utils/data/Resource.h"
#include"utils/data/Map.h"
#include"utils/data/String.h"

#include"window/Window.h"

namespace Infinity
{
	class State;

	class INFINITY_API Event
	{
	public:
		enum class EventType : unsigned char
		{
			// Handleable by client
			WindowResized, WindowClosed,
			ApplicationInterrupted,
			StateEntered, StateUpdated, StateRendered, StateExited,
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
		virtual ~Event() {}

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
	};

	// Application events

	class INFINITY_API ApplicationInterruptedEvent : public Event
	{
	public:
		ApplicationInterruptedEvent();
	};

	// State events

	class INFINITY_API StateEnteredEvent : public Event
	{
	private:
		Map<String, AnyResource> &m_resources;

	public:
		StateEnteredEvent(Map<String, AnyResource> &resources);
		
		Map<String, AnyResource> &GetResources() const;
	};

	class INFINITY_API StateUpdatedEvent : public Event
	{
	private:
		double m_dt;

	public:
		StateUpdatedEvent(double dt);
		
		double GetDT() const;
	};

	class INFINITY_API StateRenderedEvent : public Event
	{
	public:
		StateRenderedEvent();
	};

	class INFINITY_API StateExitedEvent : public Event
	{
	private:
		State *m_next_state;

	public:
		StateExitedEvent();

		template <typename T>
		void SetNextState()
		{
			static_assert(std::is_base_of<State, T>::value);

			m_next_state = new T;
		}

		State *GetNextState() const;
	};

	// keyboard events

	class INFINITY_API KeyPressedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyPressedEvent(KeyCode key);

		KeyCode GetKey() const;
	};

	class INFINITY_API KeyReleasedEvent : public Event
	{
	private:
		KeyCode m_key;

	public:
		KeyReleasedEvent(KeyCode key);

		KeyCode GetKey() const;
	};

	// mouse events

	class INFINITY_API MousePressedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MousePressedEvent(MouseCode button);

		MouseCode GetButton() const;
	};

	class INFINITY_API MouseReleasedEvent : public Event
	{
	private:
		MouseCode m_button;

	public:
		MouseReleasedEvent(MouseCode button);

		MouseCode GetButton() const;
	};

	// cursor events

	class INFINITY_API CursorEnteredEvent : public Event
	{
	private:
		Resource<Window> m_window;

	public:
		CursorEnteredEvent(Resource<Window> window);

		Resource<Window> GetWindow() const;
	};

	class INFINITY_API CursorExitedEvent : public Event
	{
	private:
		Resource<Window> m_window;

	public:
		CursorExitedEvent(Resource<Window> window);

		Resource<Window> GetWindow() const;
	};

	class INFINITY_API CursorMovedEvent : public Event
	{
	private:
		Resource<Window> m_window;

		int m_cx, m_cy;

	public:
		CursorMovedEvent(int cx, int cy, Resource<Window> window);

		Resource<Window> GetWindow() const;

		int GetCX() const;
		int GetCY() const;
	};
}