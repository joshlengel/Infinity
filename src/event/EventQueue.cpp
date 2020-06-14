#include"InfinityPCH.h"

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
		m_queue.Push(event);
	}

	Event *EventQueue::PopEvent()
	{
		if (m_queue.Empty()) return nullptr;

		Event *e = m_queue.Peek();
		m_queue.Pop();

		return e;
	}
}