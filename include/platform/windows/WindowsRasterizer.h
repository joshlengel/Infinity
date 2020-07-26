#pragma once

#ifdef INFINITY_WINDOWS

#include"Core.h"

#include"Rasterizer.h"

#include"utils/data/Resource.h"

struct ID3D11RasterizerState;
struct ID3D11BlendState;

namespace Infinity
{
	class WindowsRasterizer : public Rasterizer, public ResourceFromThis<WindowsRasterizer>
	{
	private:
		ID3D11RasterizerState *m_raster_state;
		ID3D11BlendState *m_blend_state;

	public:
		WindowsRasterizer();
		~WindowsRasterizer();

		bool Init(const RasterizerParams &params) override;

		void Bind() override;
	};
}

#endif // INFINITY_WINDOWS