#include"InfinityPCH.h"

#include"window/BaseWindow.h"

namespace Infinity
{
	BaseWindow::BaseWindow():
		m_auto_swap_buffers(),
		m_context(nullptr),
		m_device_states()
	{
		m_device_states[(unsigned int)InputDevice::Cursor] = InputDeviceState::CursorEnabled;
	}

	BaseWindow::~BaseWindow() {}

	void BaseWindow::MakeContextCurrent() const { s_context = m_context; }

	bool BaseWindow::AutoSwapBuffers() const { return m_auto_swap_buffers; }
}