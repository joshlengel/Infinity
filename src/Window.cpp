#include"Window.h"

namespace Infinity
{
	void *Window::native_context = nullptr;

	Window::Window() {}
	Window::~Window() {}

	void *Window::GetNativeContext() { return native_context; }
}