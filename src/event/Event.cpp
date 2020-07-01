#include"InfinityPCH.h"

#include"event/Event.h"

#include"window/Window.h"

namespace Infinity
{
	// Event

#ifdef DEBUG
	Event::Event(Event::EventType type, const String &debug_name):
		m_type(type),
		m_consumed(false),
		m_debug_name(debug_name)
	{}

	Event::Event(Event::EventType type, String &&debug_name):
		m_type(type),
		m_consumed(false),
		m_debug_name(std::move(debug_name))
	{}

	const String &Event::GetDebugName() const { return m_debug_name; }
#else
	Event::Event(Event::EventType type):
		m_type(type),
		m_consumed(false)
	{}
#endif // DEBUG

	Event::~Event()
	{}

	Event::EventType Event::GetType() const { return m_type; }
	void Event::Consume() { m_consumed = true; }
	bool Event::IsConsumed() const { return m_consumed; }

	// WindowResizedEvent

#ifdef DEBUG
	WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height, Resource<Window> window):
		Event(EventType::WindowResized, "WindowResizedEvent"),
		m_window(window),
		m_width(width),
		m_height(height)
	{}
#else
	WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height, Resource<Window> window):
		Event(EventType::WindowResized),
		m_window(window),
		m_width(width),
		m_height(height)
	{}
#endif // DEBUG

	WindowResizedEvent::~WindowResizedEvent()
	{}

	Resource<Window> WindowResizedEvent::GetWindow() const { return m_window; }

	unsigned int WindowResizedEvent::GetWidth() const { return m_width; }
	unsigned int WindowResizedEvent::GetHeight() const { return m_height; }

	// WindowClosedEvent

#ifdef DEBUG
	WindowClosedEvent::WindowClosedEvent(Resource<Window> window):
		Event(EventType::WindowClosed, "WindowClosedEvent"),
		m_window(window)
	{}
#else
	WindowClosedEvent::WindowClosedEvent(Resource<Window> window):
		Event(EventType::WindowClosed),
		m_window(window)
	{}
#endif // DEBUG

	WindowClosedEvent::~WindowClosedEvent()
	{}

	Resource<Window> WindowClosedEvent::GetWindow() const { return m_window; }

	// ApplicationEnteredEvent

#ifdef DEBUG
	ApplicationEnteredEvent::ApplicationEnteredEvent():
		Event(EventType::ApplicationEntered, "ApplicationEnteredEvent")
	{}
#else
	ApplicationEnteredEvent::ApplicationEnteredEvent():
		Event(EventType::ApplicationEntered)
	{}
#endif // DEBUG

	ApplicationEnteredEvent::~ApplicationEnteredEvent()
	{}

	Window::MainWindowParams &ApplicationEnteredEvent::GetMainWindowParams() { return m_params; }

	// ApplicationExitedEvent

#ifdef DEBUG
	ApplicationExitedEvent::ApplicationExitedEvent():
		Event(EventType::ApplicationExited, "ApplicationExitedEvent")
	{}
#else
	ApplicationExitedEvent::ApplicationExitedEvent():
		Event(EventType::ApplicationExited)
	{}
#endif // DEBUG

	ApplicationExitedEvent::~ApplicationExitedEvent()
	{}

	// UserCreateEvent

#ifdef DEBUG
	UserCreateEvent::UserCreateEvent():
		Event(EventType::UserCreate, "UserCreateEvent")
	{}
#else
	UserCreateEvent::UserCreateEvent():
		Event(EventType::UserCreate)
	{}
#endif // DEBUG

	UserCreateEvent::~UserCreateEvent()
	{}

	// UserUpdateEvent

#ifdef DEBUG
	UserUpdateEvent::UserUpdateEvent(double dt):
		Event(EventType::UserUpdate, "UserUpdateEvent"),
		m_dt(dt)
	{}
#else
	UserUpdateEvent::UserUpdateEvent(double dt):
		Event(EventType::UserUpdate),
		m_dt(dt)
	{}
#endif // DEBUG

	UserUpdateEvent::~UserUpdateEvent()
	{}

	double UserUpdateEvent::GetDT() const { return m_dt; }

	// UserRenderEvent

#ifdef DEBUG
	UserRenderEvent::UserRenderEvent():
		Event(EventType::UserRender, "UserRenderEvent")
	{}
#else
	UserRenderEvent::UserRenderEvent():
		Event(EventType::UserRender)
	{}
#endif // DEBUG

	UserRenderEvent::~UserRenderEvent()
	{}

	// UserDestroyEvent

#ifdef DEBUG
	UserDestroyEvent::UserDestroyEvent():
		Event(EventType::UserDestroy, "UserDestroyEvent")
	{}
#else
	UserDestroyEvent::UserDestroyEvent():
		Event(EventType::UserDestroy)
	{}
#endif // DEBUG

	UserDestroyEvent::~UserDestroyEvent()
	{}

	// KeyPressedEvent

#ifdef DEBUG
	KeyPressedEvent::KeyPressedEvent(KeyCode key):
		Event(EventType::KeyPressed, "KeyPressedEvent"),
		m_key(key)
	{}
#else
	KeyPressedEvent::KeyPressedEvent(KeyCode key):
		Event(EventType::KeyPressed),
		m_key(key)
	{}
#endif // DEBUG

	KeyPressedEvent::~KeyPressedEvent()
	{}

	KeyCode KeyPressedEvent::GetKey() const { return m_key; }

	// KeyReleasedEvent

#ifdef DEBUG
	KeyReleasedEvent::KeyReleasedEvent(KeyCode key):
		Event(EventType::KeyReleased, "KeyReleasedEvent"),
		m_key(key)
	{}
#else
	KeyReleasedEvent::KeyReleasedEvent(KeyCode key):
		Event(EventType::KeyReleased),
		m_key(key)
	{}
#endif // DEBUG

	KeyReleasedEvent::~KeyReleasedEvent()
	{}

	KeyCode KeyReleasedEvent::GetKey() const { return m_key; }

	// MousePressedEvent

#ifdef DEBUG
	MousePressedEvent::MousePressedEvent(MouseCode button):
		Event(EventType::MousePressed, "MousePressedEvent"),
		m_button(button)
	{}
#else
	MousePressedEvent::MousePressedEvent(MouseCode button):
		Event(EventType::MousePressed),
		m_button(button)
	{}
#endif // DEBUG
	
	MousePressedEvent::~MousePressedEvent()
	{}

	MouseCode MousePressedEvent::GetButton() const { return m_button; }

	// MouseReleasedEvent

#ifdef DEBUG
	MouseReleasedEvent::MouseReleasedEvent(MouseCode button):
		Event(EventType::MouseReleased, "MouseReleasedEvent"),
		m_button(button)
	{}
#else
	MouseReleasedEvent::MouseReleasedEvent(MouseCode button):
		Event(EventType::MouseReleased),
		m_button(button)
	{}
#endif // DEBUG

	MouseReleasedEvent::~MouseReleasedEvent()
	{}

	MouseCode MouseReleasedEvent::GetButton() const { return m_button; }

	// CursorEnteredEvent

#ifdef DEBUG
	CursorEnteredEvent::CursorEnteredEvent(Resource<Window> window):
		Event(EventType::CursorEntered, "CursorEnteredEvent"),
		m_window(window)
	{}
#else
	CursorEnteredEvent::CursorEnteredEvent(Resource<Window> window):
		Event(EventType::CursorEntered),
		m_window(window)
	{}
#endif // DEBUG

	CursorEnteredEvent::~CursorEnteredEvent()
	{}

	Resource<Window> CursorEnteredEvent::GetWindow() const { return m_window; }

	// CursorExitedEvent

#ifdef DEBUG
	CursorExitedEvent::CursorExitedEvent(Resource<Window> window):
		Event(EventType::CursorExited, "CursorExitedEvent"),
		m_window(window)
	{}
#else
	CursorExitedEvent::CursorExitedEvent(Resource<Window> window):
		Event(EventType::CursorExited),
		m_window(window)
	{}
#endif // DEBUG

	CursorExitedEvent::~CursorExitedEvent()
	{}

	Resource<Window> CursorExitedEvent::GetWindow() const { return m_window; }

	// CursorMovedEvent

#ifdef DEBUG
	CursorMovedEvent::CursorMovedEvent(int cx, int cy, Resource<Window> window):
		Event(EventType::CursorMoved, "CursorMovedEvent"),
		m_window(window),
		m_cx(cx),
		m_cy(cy)
	{}
#else
	CursorMovedEvent::CursorMovedEvent(int cx, int cy, Resource<Window> window):
		Event(EventType::CursorMoved),
		m_window(window),
		m_cx(cx),
		m_cy(cy)
	{}
#endif // DEBUG

	CursorMovedEvent::~CursorMovedEvent()
	{}

	Resource<Window> CursorMovedEvent::GetWindow() const { return m_window; }

	int CursorMovedEvent::GetCX() const { return m_cx; }
	int CursorMovedEvent::GetCY() const { return m_cy; }
}