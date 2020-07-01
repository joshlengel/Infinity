#pragma once

#include"Core.h"

#include"Event.h"

#include"utils/data/Queue.h"

namespace Infinity
{
	class EventQueue
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