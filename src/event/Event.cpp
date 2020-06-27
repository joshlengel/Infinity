#include"InfinityPCH.h"

#include"event/Event.h"

namespace Infinity
{
	// Event

#ifdef DEBUG
	Event::Event(Event::EventType type, const String &debug_name, void *caller):
		m_type(type),
		m_consumed(false),
		m_debug_name(debug_name),
		m_caller(caller)
	{}

	Event::Event(Event::EventType type, String &&debug_name, void *caller):
		m_type(type),
		m_consumed(false),
		m_debug_name(std::move(debug_name)),
		m_caller(caller)
	{}

	const String &Event::GetDebugName() const { return m_debug_name; }
#else
	Event::Event(Event::EventType type, void *caller):
		m_type(type),
		m_consumed(false),
		m_caller(caller)
	{}
#endif // DEBUG

	Event::~Event()
	{}

	Event::EventType Event::GetType() const { return m_type; }
	void Event::Consume() { m_consumed = true; }
	bool Event::IsConsumed() const { return m_consumed; }

	void *Event::GetCaller() const { return m_caller; }

	// WindowResizedEvent

#ifdef DEBUG
	WindowResizedEvent::WindowResizedEvent(Window *window, unsigned int width, unsigned int height, void *caller):
		Event(EventType::WindowResized, "WindowResizedEvent", caller),
		m_window(window),
		m_width(width),
		m_height(height)
	{}
#else
	WindowResizedEvent::WindowResizedEvent(Window *window, unsigned int width, unsigned int height, void *caller):
		Event(EventType::WindowResized, caller),
		m_window(window),
		m_width(width),
		m_height(height)
	{}
#endif // DEBUG

	WindowResizedEvent::~WindowResizedEvent()
	{}

	Window *WindowResizedEvent::GetWindow() const { return m_window; }

	unsigned int WindowResizedEvent::GetWidth() const { return m_width; }
	unsigned int WindowResizedEvent::GetHeight() const { return m_height; }

	// WindowClosedEvent

#ifdef DEBUG
	WindowClosedEvent::WindowClosedEvent(Window *window, void *caller):
		Event(EventType::WindowClosed, "WindowClosedEvent", caller),
		m_window(window)
	{}
#else
	WindowClosedEvent::WindowClosedEvent(Window *window, void *caller):
		Event(EventType::WindowClosed, caller),
		m_window(window)
	{}
#endif // DEBUG

	WindowClosedEvent::~WindowClosedEvent()
	{}

	Window *WindowClosedEvent::GetWindow() const { return m_window; }

	// ApplicationEnteredEvent

#ifdef DEBUG
	ApplicationEnteredEvent::ApplicationEnteredEvent(void *caller):
		Event(EventType::ApplicationEntered, "ApplicationEnteredEvent", caller),
		m_params()
	{}
#else
	ApplicationEnteredEvent::ApplicationEnteredEvent(void *caller):
		Event(EventType::ApplicationEntered, caller),
		m_params()
	{}
#endif // DEBUG

	ApplicationEnteredEvent::~ApplicationEnteredEvent()
	{}

	Window::WindowParams &ApplicationEnteredEvent::GetMainWindowParams() { return m_params; }

	// ApplicationExitedEvent

#ifdef DEBUG
	ApplicationExitedEvent::ApplicationExitedEvent(void *caller):
		Event(EventType::ApplicationExited, "ApplicationExitedEvent", caller)
	{}
#else
	ApplicationExitedEvent::ApplicationExitedEvent(void *caller):
		Event(EventType::ApplicationExited, caller)
	{}
#endif // DEBUG

	ApplicationExitedEvent::~ApplicationExitedEvent()
	{}

	// UserCreateEvent

#ifdef DEBUG
	UserCreateEvent::UserCreateEvent(void *caller):
		Event(EventType::UserCreate, "UserCreateEvent", caller)
	{}
#else
	UserCreateEvent::UserCreateEvent(void *caller):
		Event(EventType::UserCreate, caller),
	{}
#endif // DEBUG

	UserCreateEvent::~UserCreateEvent()
	{}

	// UserUpdateEvent

#ifdef DEBUG
	UserUpdateEvent::UserUpdateEvent(double dt, void *caller):
		Event(EventType::UserUpdate, "UserUpdateEvent", caller),
		m_dt(dt)
	{}
#else
	UserUpdateEvent::UserUpdateEvent(double dt, void *caller):
		Event(EventType::UserUpdate, caller),
		m_dt(dt)
	{}
#endif // DEBUG

	UserUpdateEvent::~UserUpdateEvent()
	{}

	double UserUpdateEvent::GetDT() const { return m_dt; }

	// UserRenderEvent

#ifdef DEBUG
	UserRenderEvent::UserRenderEvent(void *caller):
		Event(EventType::UserRender, "UserRenderEvent", caller)
	{}
#else
	UserRenderEvent::UserRenderEvent(void *caller):
		Event(EventType::UserRender, caller)
	{}
#endif // DEBUG

	UserRenderEvent::~UserRenderEvent()
	{}

	// UserDestroyEvent

#ifdef DEBUG
	UserDestroyEvent::UserDestroyEvent(void *caller):
		Event(EventType::UserDestroy, "UserDestroyEvent", caller)
	{}
#else
	UserDestroyEvent::UserDestroyEvent(void *caller):
		Event(EventType::UserDestroy, caller)
	{}
#endif // DEBUG

	UserDestroyEvent::~UserDestroyEvent()
	{}

	// KeyPressedEvent

#ifdef DEBUG
	KeyPressedEvent::KeyPressedEvent(KeyCode key, void *caller):
		Event(EventType::KeyPressed, "KeyPressedEvent", caller),
		m_key(key)
	{}
#else
	KeyPressedEvent::KeyPressedEvent(KeyCode key, void *caller):
		Event(EventType::KeyPressed, caller),
		m_key(key)
	{}
#endif // DEBUG

	KeyPressedEvent::~KeyPressedEvent()
	{}

	KeyCode KeyPressedEvent::GetKey() const { return m_key; }

	// KeyReleasedEvent

#ifdef DEBUG
	KeyReleasedEvent::KeyReleasedEvent(KeyCode key, void *caller):
		Event(EventType::KeyReleased, "KeyReleasedEvent", caller),
		m_key(key)
	{}
#else
	KeyReleasedEvent::KeyReleasedEvent(KeyCode key, void *caller):
		Event(EventType::KeyReleased, caller),
		m_key(key)
	{}
#endif // DEBUG

	KeyReleasedEvent::~KeyReleasedEvent()
	{}

	KeyCode KeyReleasedEvent::GetKey() const { return m_key; }

	// MousePressedEvent

#ifdef DEBUG
	MousePressedEvent::MousePressedEvent(MouseCode button, void *caller):
		Event(EventType::MousePressed, "MousePressedEvent", caller),
		m_button(button)
	{}
#else
	MousePressedEvent::MousePressedEvent(MouseCode button, void *caller):
		Event(EventType::MousePressed, caller),
		m_button(button)
	{}
#endif // DEBUG
	
	MousePressedEvent::~MousePressedEvent()
	{}

	MouseCode MousePressedEvent::GetButton() const { return m_button; }

	// MouseReleasedEvent

#ifdef DEBUG
	MouseReleasedEvent::MouseReleasedEvent(MouseCode button, void *caller):
		Event(EventType::MouseReleased, "MouseReleasedEvent", caller),
		m_button(button)
	{}
#else
	MouseReleasedEvent::MouseReleasedEvent(MouseCode button, void *caller):
		Event(EventType::MouseReleased, caller),
		m_button(button)
	{}
#endif // DEBUG

	MouseReleasedEvent::~MouseReleasedEvent()
	{}

	MouseCode MouseReleasedEvent::GetButton() const { return m_button; }

	// CursorEnteredEvent

#ifdef DEBUG
	CursorEnteredEvent::CursorEnteredEvent(void *caller):
		Event(EventType::CursorEntered, "CursorEnteredEvent", caller)
	{}
#else
	CursorEnteredEvent::CursorEnteredEvent(void *caller):
		Event(EventType::CursorEntered, caller)
	{}
#endif // DEBUG

	CursorEnteredEvent::~CursorEnteredEvent()
	{}

	// CursorExitedEvent

#ifdef DEBUG
	CursorExitedEvent::CursorExitedEvent(void *caller):
		Event(EventType::CursorExited, "CursorExitedEvent", caller)
	{}
#else
	CursorExitedEvent::CursorExitedEvent(void *caller):
		Event(EventType::CursorExited, caller)
	{}
#endif // DEBUG

	CursorExitedEvent::~CursorExitedEvent()
	{}

	// CursorMovedEvent

#ifdef DEBUG
	CursorMovedEvent::CursorMovedEvent(int cx, int cy, void *caller):
		Event(EventType::CursorMoved, "CursorMovedEvent", caller),
		m_cx(cx),
		m_cy(cy)
	{}
#else
	CursorMovedEvent::CursorMovedEvent(int cx, int cy, void *caller):
		Event(EventType::CursorMoved, caller),
		m_cx(cx),
		m_cy(cy)
	{}
#endif // DEBUG

	CursorMovedEvent::~CursorMovedEvent()
	{}

	int CursorMovedEvent::GetCX() const { return m_cx; }
	int CursorMovedEvent::GetCY() const { return m_cy; }
}