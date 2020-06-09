#pragma once

#include"Core.h"

#include<queue>

namespace Infinity
{
	class Event;

	class INFINITY_API EventQueue
	{
	private:
		std::queue<Event*> m_queue;

	public:
		EventQueue();
		~EventQueue();

		void PushEvent(Event *event);
		Event *PopEvent();
	};
}