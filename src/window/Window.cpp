#include"InfinityPCH.h"

#include"window/Window.h"

namespace Infinity
{
	Context *Window::s_context = nullptr;

	Context *Window::GetContext() { return s_context; }
}