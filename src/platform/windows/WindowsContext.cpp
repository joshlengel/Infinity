#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsContext.h"

#include"platform/windows/WindowsWindow.h"

#include"Rasterizer.h"

constexpr const static D3D11_COMPARISON_FUNC COMPARISON_FUNCS[] =
{
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_ALWAYS
};

namespace Infinity
{
	Resource<ContextSettings> ContextSettings::CreateContextSettings()
	{
		return ResourceCast<ContextSettings>(MakeResource<WindowsContextSettings>());
	}

	WindowsContextSettings::WindowsContextSettings():
		m_depth_stencil_state(nullptr)
	{}

	WindowsContextSettings::~WindowsContextSettings()
	{
		if (m_depth_stencil_state)
		{
			m_depth_stencil_state->Release();
		}
	}

	bool WindowsContextSettings::Init(DepthMode depth_mode)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		D3D11_DEPTH_STENCIL_DESC ds_desc = {};
		ds_desc.DepthEnable = true;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		ds_desc.DepthFunc = COMPARISON_FUNCS[(unsigned int)depth_mode];

		ds_desc.StencilEnable = true;
		ds_desc.StencilReadMask = 0xff;
		ds_desc.StencilWriteMask = 0xff;

		ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;

		ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

		if (FAILED(context->GetDevice()->CreateDepthStencilState(&ds_desc, &m_depth_stencil_state)))
		{
			INFINITY_CORE_ERROR("Error creating depth stencil state");
			return false;
		}

		return true;
	}

	WindowsContext::WindowsContext(const Resource<Window> &window, ID3D11Device *device, ID3D11DeviceContext *device_context):
		Context(window),
		m_device(device),
		m_device_context(device_context),
		m_render_target_view(nullptr),
		m_depth_stencil_view(nullptr),
		m_clear_color{ 0.0f, 0.0f, 0.0f, 1.0f }
	{}

	WindowsContext::~WindowsContext()
	{}

	bool WindowsContext::Init()
	{
		auto def_settings = ContextSettings::CreateContextSettings();

		if (!def_settings->Init())
		{
			INFINITY_CORE_ERROR("Error initializing default context settings");
			return false;
		}

		SetContextSettings(def_settings);

		m_def_rasterizer = Infinity::Rasterizer::CreateRasterizer();

		if (!m_def_rasterizer->Init(Rasterizer::RasterizerParams()))
		{
			INFINITY_CORE_ERROR("Error initializing default rasterizer");
			return false;
		}

		m_def_rasterizer->Bind();

		return true;
	}

	bool WindowsContext::Resize(ID3D11RenderTargetView *render_target_view, ID3D11DepthStencilView *depth_stencil_view, unsigned int width, unsigned int height)
	{
		m_render_target_view = render_target_view;
		m_depth_stencil_view = depth_stencil_view;

		m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

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

			if (m_depth_stencil_view)
			{
				m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0x00);
			}
			else
			{
				INFINITY_CORE_ERROR("Error clearing context: Depth stencil view is null.");
			}
		}
		else
		{
			INFINITY_CORE_ERROR("Error clearing context: Device context is null.");
		}
	}

	void WindowsContext::SetContextSettings(const Resource<ContextSettings> &settings)
	{
		m_settings = settings;

		m_device_context->OMSetDepthStencilState(ResourceCast<WindowsContextSettings>(settings)->m_depth_stencil_state, 1);
	}

	ID3D11Device *WindowsContext::GetDevice() const { return m_device; }
	ID3D11DeviceContext *WindowsContext::GetDeviceContext() const { return m_device_context; }
}

#endif // INFINITY_WINDOWS