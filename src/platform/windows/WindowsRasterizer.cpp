#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsRasterizer.h"

#include"platform/windows/WindowsWindow.h"
#include"platform/windows/WindowsContext.h"

namespace Infinity
{
	Resource<Rasterizer> Rasterizer::CreateRasterizer()
	{
		return ResourceCast<Rasterizer>(MakeResource<WindowsRasterizer>());
	}

	WindowsRasterizer::WindowsRasterizer():
		m_raster_state(nullptr),
		m_blend_state(nullptr)
	{}

	WindowsRasterizer::~WindowsRasterizer()
	{
		if (m_raster_state)
		{
			m_raster_state->Release();
		}

		if (m_blend_state)
		{
			m_blend_state->Release();
		}
	}

	bool WindowsRasterizer::Init(CullMode cull, bool blend)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11Device *device = context->GetDevice();

		D3D11_RASTERIZER_DESC raster_desc = {};
		raster_desc.AntialiasedLineEnable = true;
		
		switch (cull)
		{
		case CullMode::NONE:
			raster_desc.CullMode = D3D11_CULL_NONE;
			break;

		case CullMode::CLOCKWISE:
			raster_desc.CullMode = D3D11_CULL_FRONT;
			break;

		case CullMode::COUNTERCLOCKWISE:
			raster_desc.CullMode = D3D11_CULL_BACK;
			break;
		}

		raster_desc.FrontCounterClockwise = false;

		raster_desc.DepthBias = 0;
		raster_desc.DepthBiasClamp = 0.0f;
		raster_desc.DepthClipEnable = true;

		raster_desc.FillMode = D3D11_FILL_SOLID;

		raster_desc.MultisampleEnable = true;
		raster_desc.ScissorEnable = false;

		raster_desc.SlopeScaledDepthBias = 0.0f;

		if (FAILED(device->CreateRasterizerState(&raster_desc, &m_raster_state)))
		{
			INFINITY_CORE_ERROR("Error initializing rasterizer");
			return false;
		}

		D3D11_BLEND_DESC blend_desc = {};
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;

		blend_desc.RenderTarget[0].BlendEnable = blend;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		if (FAILED(device->CreateBlendState(&blend_desc, &m_blend_state)))
		{
			INFINITY_CORE_ERROR("Error initializing rasterizer");
			return false;
		}

		return true;
	}

	void WindowsRasterizer::Bind()
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->RSSetState(m_raster_state);
		device_context->OMSetBlendState(m_blend_state, nullptr, 0xffffffff);
	}
}

#endif // INFINITY_WINDOWS
