#include"InfinityPCH.h"

#include"window/Window.h"

namespace Infinity
{
	Resource<Context> Window::s_context = nullptr;

	Resource<Context> Window::GetContext() { return s_context; }
}