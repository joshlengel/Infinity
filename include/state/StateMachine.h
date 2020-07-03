#pragma once

#include"Core.h"

#include"State.h"

#include"event/Event.h"

#include"utils/data/Map.h"
#include"utils/data/Resource.h"
#include"utils/data/String.h"

namespace Infinity
{
	class INFINITY_API StateMachine
	{
	private:
		Map<String, AnyResource> m_resources;
		State *m_current_state;

	public:
		StateMachine(State *first_state);

		void Start();
		void PushEvents(double dt);

		bool ShouldExit() const;

	private:
		void EventHandler(Event &event);
	};

	INFINITY_TEMPLATE template class INFINITY_API Map<String, AnyResource>;
}