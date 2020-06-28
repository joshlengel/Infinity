#include"InfinityPCH.h"

#include"window/Window.h"
#include"window/BaseWindow.h"

#include"Application.h"

#include"event/Event.h"

namespace Infinity
{
	Context *Window::s_context = nullptr;

	Context *Window::GetContext() { return s_context; }
}