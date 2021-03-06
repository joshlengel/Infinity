#include"InfinityPCH.h"

#include"EntryPoint.h"

#include"state/State.h"

#include"window/Window.h"

#include"application/BaseApplication.h"

namespace Infinity
{
	namespace _Impl
	{
		Window::MainWindowParams main_window_params;

		State *start_state;
	}

	void ExecuteMain(State *start_state)
	{
		BaseApplication base_app(start_state);

		base_app.Run();
	}
}