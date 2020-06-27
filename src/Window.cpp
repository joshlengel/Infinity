#include"InfinityPCH.h"

#include"Window.h"

namespace Infinity
{
	Window *Window::main_window = nullptr;
	Context *Window::context = nullptr;

	Window::Window():
		m_auto_swap_buffers(),
		m_context(nullptr)
	{}

	Window::~Window() {}

	void Window::MakeContextCurrent()
	{
		context = m_context;
	}

	bool Window::AutoSwapBuffers() const { return m_auto_swap_buffers; }

	Context *Window::GetContext() { return context; }
}