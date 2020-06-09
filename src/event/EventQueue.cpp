#include"event/EventQueue.h"

namespace Infinity
{
	EventQueue::EventQueue():
		m_queue()
	{}

	EventQueue::~EventQueue()
	{}

	void EventQueue::PushEvent(Event *event)
	{
		m_queue.push(event);
	}

	Event *EventQueue::PopEvent()
	{
		if (m_queue.empty()) return nullptr;

		Event *e = m_queue.front();
		m_queue.pop();

		return e;
	}
}