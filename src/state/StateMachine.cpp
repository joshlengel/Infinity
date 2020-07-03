#include"InfinityPCH.h"

#include"state/StateMachine.h"

#include"application/BaseApplication.h"

namespace Infinity
{
	void DefaultStateDeleter(State *state) { delete state; }

	StateMachine::StateMachine(State *first_state):
		m_current_state(first_state),
		m_resources()
	{
		BaseApplication::GetApplication()->AddEventHandler(INFINITY_TO_STATIC_EVENT_FUNC(StateMachine::EventHandler));
	}

	void StateMachine::Start()
	{
		BaseApplication::GetApplication()->PushEvent(new StateEnteredEvent(m_resources));
	}

	void StateMachine::PushEvents(double dt)
	{
		if (m_current_state->m_exit)
		{
			BaseApplication::GetApplication()->PushEvent(new StateExitedEvent(m_resources));
		}
		else
		{
			BaseApplication::GetApplication()->PushEvent(new StateUpdatedEvent(dt));
			BaseApplication::GetApplication()->PushEvent(new StateRenderedEvent);
		}
	}

	bool StateMachine::ShouldExit() const { return m_current_state == nullptr; }

	void StateMachine::EventHandler(Event &event)
	{
		switch (event.GetType())
		{
		case Event::EventType::StateEntered:
			m_current_state->OnStateEntered((StateEnteredEvent&)event);

			m_resources.Clear();

			break;

		case Event::EventType::StateUpdated:
			m_current_state->OnStateUpdated((StateUpdatedEvent&)event);
			break;

		case Event::EventType::StateRendered:
			m_current_state->OnStateRendered((StateRenderedEvent&)event);
			break;

		case Event::EventType::StateExited:
		{
			StateExitedEvent &see = (StateExitedEvent&)event;
			m_current_state->OnStateExited(see);
			
			// This wierd code lets the DLL delete the first state which is created in the executable (Client) code
			void(*deleter)(State*) = m_current_state->m_deleter;
			deleter(m_current_state);

			m_current_state = see.GetNextState();

			if (m_current_state)
			{
				m_current_state->m_deleter = DefaultStateDeleter;

				BaseApplication::GetApplication()->PushEvent(new StateEnteredEvent(m_resources));
			}

			break;
		}
		case Event::EventType::ApplicationInterrupted:
			if (m_current_state)
			{
				StateExitedEvent see(m_resources);
				m_current_state->OnStateExited(see);

				if (see.GetNextState()) delete see.GetNextState();

				delete m_current_state;
				m_current_state = nullptr;
			}

		case Event::EventType::WindowResized:
			if (m_current_state)
				m_current_state->OnWindowResized((WindowResizedEvent&)event);
			break;

		case Event::EventType::WindowClosed:
			if (m_current_state)
				m_current_state->OnWindowClosed((WindowClosedEvent&)event);
			break;

		case Event::EventType::AttemptWindowClosed:
			if (m_current_state)
			{
				AttemptWindowClosedEvent &awce = (AttemptWindowClosedEvent&)event;
				m_current_state->OnAttemptWindowClosed(awce);
				
				if (awce.AllowedClose())
				{
					BaseApplication::GetApplication()->PushEvent(new WindowClosedEvent(awce.GetWindow()));
				}
			}
			break;

		case Event::EventType::KeyPressed:
			if (m_current_state)
				m_current_state->OnKeyPressed((KeyPressedEvent&)event);
			break;

		case Event::EventType::KeyReleased:
			if (m_current_state)
				m_current_state->OnKeyReleased((KeyReleasedEvent&)event);
			break;

		case Event::EventType::MousePressed:
			if (m_current_state)
				m_current_state->OnMousePressed((MousePressedEvent&)event);
			break;

		case Event::EventType::MouseReleased:
			if (m_current_state)
				m_current_state->OnMouseReleased((MouseReleasedEvent&)event);
			break;

		case Event::EventType::CursorMoved:
			if (m_current_state)
				m_current_state->OnCursorMoved((CursorMovedEvent&)event);
			break;

		case Event::EventType::CursorEntered:
			if (m_current_state)
				m_current_state->OnCursorEntered((CursorEnteredEvent&)event);
			break;

		case Event::EventType::CursorExited:
			if (m_current_state)
				m_current_state->OnCursorExited((CursorExitedEvent&)event);
			break;
		}
	}
}