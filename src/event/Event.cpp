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

	Resource<Window> WindowClosedEvent::GetWindow() const { return m_window; }

	// AttemptWindowClosedEvent

#ifdef DEBUG
	AttemptWindowClosedEvent::AttemptWindowClosedEvent(Resource<Window> window):
		Event(EventType::AttemptWindowClosed, "AttemptWindowClosedEvent"),
		m_window(window),
		m_allow_close(true)
	{}
#else
	AttemptWindowClosedEvent::AttemptWindowClosedEvent(Resource<Window> window):
		Event(EventType::AttemptWindowClosed),
		m_window(window),
		m_allow_close(true)
	{}
#endif // DEBUG

	Resource<Window> AttemptWindowClosedEvent::GetWindow() const { return m_window; }

	void AttemptWindowClosedEvent::AllowClose(bool allow_close) { m_allow_close = allow_close; }
	bool AttemptWindowClosedEvent::AllowedClose() const { return m_allow_close; }

	// ApplicationInterruptedEvent

#ifdef DEBUG
	ApplicationInterruptedEvent::ApplicationInterruptedEvent():
		Event(EventType::ApplicationInterrupted, "ApplicationInterruptedEvent")
	{}
#else
	ApplicationInterruptedEvent::ApplicationInterruptedEvent():
		Event(EventType::ApplicationInterrupted)
	{}
#endif

	// StateEnteredEvent

#ifdef DEBUG
	StateEnteredEvent::StateEnteredEvent(const Map<String, AnyResource> &resources):
		Event(EventType::StateEntered, "StateEnteredEvent"),
		m_resources(resources)
	{}
#else
	StateEnteredEvent::StateEnteredEvent(const Map<String, AnyResource> &resources):
		Event(EventType::StateEntered),
		m_resources(resources)
	{}
#endif // DEBUG

	const Map<String, AnyResource> &StateEnteredEvent::GetResources() const { return m_resources; }

	// StateUpdatedEvent

#ifdef DEBUG
	StateUpdatedEvent::StateUpdatedEvent(double dt):
		Event(EventType::StateUpdated, "StateUpdatedEvent"),
		m_dt(dt)
	{}
#else
	StateUpdatedEvent::StateUpdatedEvent(double dt):
		Event(EventType::StateUpdated),
		m_dt(dt)
	{}
#endif // DEBUG

	double StateUpdatedEvent::GetDT() const { return m_dt; }

	// StateRenderedEvent

#ifdef DEBUG
	StateRenderedEvent::StateRenderedEvent():
		Event(EventType::StateRendered, "StateRenderedEvent")
	{}
#else
	StateRenderedEvent::StateRenderedEvent():
		Event(EventType::StateRendered)
	{}
#endif // DEBUG

	// StateExitedEvent

#ifdef DEBUG
	StateExitedEvent::StateExitedEvent(Map<String, AnyResource> &resources):
		Event(EventType::StateExited, "StateExitedEvent"),
		m_resources(resources),
		m_next_state(nullptr)
	{}
#else
	StateExitedEvent::StateExitedEvent(Map<String, AnyResource> &resources):
		Event(EventType::StateExited),
		m_resources(resources),
		m_next_state(nullptr)
	{}
#endif // DEBUG

	Map<String, AnyResource> &StateExitedEvent::GetResources() const { return m_resources; }

	State *StateExitedEvent::GetNextState() const { return m_next_state; }

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

	Resource<Window> CursorMovedEvent::GetWindow() const { return m_window; }

	int CursorMovedEvent::GetCX() const { return m_cx; }
	int CursorMovedEvent::GetCY() const { return m_cy; }
}