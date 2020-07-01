#pragma once

#ifdef INFINITY_WINDOWS

#include"Core.h"

#include"Rasterizer.h"

struct ID3D11RasterizerState;
struct ID3D11BlendState;

namespace Infinity
{
	class WindowsRasterizer : public Rasterizer
	{
	private:
		ID3D11RasterizerState *m_raster_state;
		ID3D11BlendState *m_blend_state;

	public:
		WindowsRasterizer();
		~WindowsRasterizer();

		bool Init(CullMode cull, bool blend) override;

		void Bind() override;
	};
}

#endif // INFINITY_WINDOWS