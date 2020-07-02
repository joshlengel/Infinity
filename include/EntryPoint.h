#pragma once

#include"state/State.h"

namespace Infinity
{
	namespace _Impl
	{
		extern INFINITY_API State *start_state;

		inline void StartStateDeleter(State *state) { delete state; }
	}

	template <typename T>
	inline void SetStartState()
	{
		static_assert(std::is_base_of<State, T>::value);

		if (!_Impl::start_state)
		{
			_Impl::start_state = new T;
			_Impl::start_state->m_deleter = _Impl::StartStateDeleter;
		}
	}

	void SetClientStartState();

	void INFINITY_API ExecuteMain(State *start_state);
}

#ifdef INFINITY_ENTRY_POINT

int main(int argc, char **argv)
{
#ifdef DEBUG
	Infinity::Log::Init();
#endif // DEBUG

	Infinity::SetClientStartState();

#ifdef DEBUG
	if (!Infinity::_Impl::start_state)
	{
		INFINITY_CLIENT_FATAL("No start state found. Set start state via Infinity::SetStartState<T>() in Infinity::SetClientStartState()");
	}
#endif // DEBUG

	Infinity::ExecuteMain(Infinity::_Impl::start_state);
}

#endif // INFINITY_ENTRY_POINT