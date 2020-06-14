#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsContext.h"

#include"platform/windows/WindowsWindow.h"

namespace Infinity
{
	Context *Context::CreateContext()
	{
		return new WindowsContext;
	}

	WindowsContext::WindowsContext():
		m_device_context(nullptr),
		m_render_target_view(nullptr),
		m_clear_color{ 0.0f, 0.0f, 0.0f, 1.0f }
	{}

	WindowsContext::~WindowsContext()
	{}

	bool WindowsContext::Init()
	{
		if (!Window::GetNativeContext())
		{
			INFINITY_CORE_ERROR("No window context bound when initializing context. Make sure to call window->MakeContextCurrent before calling this function.");
			return false;
		}

		WindowsWindow::WindowsWindowContext *context = (WindowsWindow::WindowsWindowContext*)(Window::GetNativeContext());

		m_device_context = context->device_context;
		m_render_target_view = context->render_target_view;
		context->context = this;
		context->render_target_view = nullptr;

		return true;
	}

	void WindowsContext::Destroy()
	{
		WindowsWindow::WindowsWindowContext *context = (WindowsWindow::WindowsWindowContext*)(Window::GetNativeContext());

		if (context && context->context == this)
		{
			context->context = nullptr;
		}

		m_device_context = nullptr;
		m_render_target_view = nullptr;
	}

	bool WindowsContext::Resize(ID3D11RenderTargetView *render_target_view, unsigned int width, unsigned int height)
	{
		m_render_target_view = render_target_view;

		m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;

		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_device_context->RSSetViewports(1, &viewport);

		return true;
	}

	void WindowsContext::SetClearColor(float r, float g, float b, float a)
	{
		m_clear_color[0] = r;
		m_clear_color[1] = g;
		m_clear_color[2] = b;
		m_clear_color[3] = a;
	}

	void WindowsContext::Clear()
	{
		if (m_device_context)
		{
			if (m_render_target_view)
			{
				m_device_context->ClearRenderTargetView(m_render_target_view, m_clear_color);
			}
			else
			{
				INFINITY_CORE_ERROR("Error clearing context: Render target view is null.");
			}
		}
		else
		{
			INFINITY_CORE_ERROR("Error clearing context: Device context is null.");
		}
	}
}

#endif // INFINITY_WINDOWS