#include"InfinityPCH.h"

#include"state/State.h"

#include"application/BaseApplication.h"

namespace Infinity
{
	State::State():
		m_deleter(nullptr),
		m_exit(false)
	{}

	void State::RequestExit() { m_exit = true; }

	const WindowSystem &State::GetWindowSystem() { return BaseApplication::GetApplication()->GetWindowSystem(); }
}