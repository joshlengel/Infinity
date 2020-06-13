#pragma once

#include"Core.h"

#include"utils/data/Queue.h"

namespace Infinity
{
	class Event;

	INFINITY_TEMPLATE template class INFINITY_API Queue<Event*>;

	class INFINITY_API EventQueue
	{
	private:
		Queue<Event*> m_queue;

	public:
		EventQueue();
		~EventQueue();

		void PushEvent(Event *event);
		Event *PopEvent();
	};
}