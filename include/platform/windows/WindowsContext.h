#pragma once

#ifdef INFINITY_WINDOWS

#include"Core.h"
#include"Context.h"

struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3DDepthStencilView;

namespace Infinity
{
	class INFINITY_API WindowsContext : public Context
	{
	private:
		ID3D11DeviceContext *m_device_context;
		ID3D11RenderTargetView *m_render_target_view;
		ID3D11DepthStencilView *m_depth_stencil_view;

		float m_clear_color[4];

	public:
		WindowsContext();
		~WindowsContext();

		bool Init() override;
		void Destroy() override;

		bool Resize(ID3D11RenderTargetView *render_target_view, ID3D11DepthStencilView *depth_stencil_view, unsigned int width, unsigned int height);

		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
	};
}

#endif // INFINITY_WINDOWS