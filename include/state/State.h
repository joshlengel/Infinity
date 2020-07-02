#pragma once

#include"Core.h"

#include"event/Event.h"

#include"utils/data/Map.h"
#include"utils/data/Resource.h"
#include"utils/data/String.h"

#include"window/WindowSystem.h"

namespace Infinity
{
	class StateMachine;

	class INFINITY_API State
	{
		// Allow StateMachine to set m_exit flag
		friend StateMachine;
	
		// Allow SetStartState to set deleter
		template <typename T>
		friend void SetStartState();

	private:
		bool m_exit = false;

		void(*m_deleter)(State*);

	public:
		State();
		virtual ~State() {}

		virtual void OnStateEntered(StateEnteredEvent &event) = 0;
		virtual void OnStateUpdated(StateUpdatedEvent &event) = 0;
		virtual void OnStateRendered(StateRenderedEvent &event) = 0;
		virtual void OnStateExited(StateExitedEvent &event) = 0;

		virtual void OnWindowResized(WindowResizedEvent &event) {}
		virtual void OnWindowClosed(WindowClosedEvent &event) {}

		virtual void OnKeyPressed(KeyPressedEvent &event) {}
		virtual void OnKeyReleased(KeyReleasedEvent &event) {}
		virtual void OnMousePressed(MousePressedEvent &event) {}
		virtual void OnMouseReleased(MouseReleasedEvent &event) {}

		virtual void OnCursorMoved(CursorMovedEvent &event) {}
		virtual void OnCursorEntered(CursorEnteredEvent &event) {}
		virtual void OnCursorExited(CursorExitedEvent &event) {}


	protected:
		void RequestExit();
		const WindowSystem &GetWindowSystem();
	};
}