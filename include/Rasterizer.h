#pragma once

#include"Core.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	class Rasterizer;

	INFINITY_TEMPLATE template class INFINITY_API Resource<Rasterizer>;
	INFINITY_TEMPLATE template class INFINITY_API ResourceView<Rasterizer>;

	class INFINITY_API Rasterizer
	{
	public:
		enum class CullMode : unsigned char
		{
			CLOCKWISE,
			COUNTERCLOCKWISE,
			NONE
		};

		struct RasterizerParams
		{
			CullMode cull_mode = CullMode::NONE;
			bool blend_alpha = false;
		};

	protected:
		inline static ResourceView<Rasterizer> s_rasterizer = nullptr;

	public:
		virtual ~Rasterizer() {}

		virtual bool Init(const RasterizerParams &params) = 0;
		
		virtual void Bind() = 0;

		static Resource<Rasterizer> CreateRasterizer();
		static ResourceView<Rasterizer> GetRasterizer() { return s_rasterizer; }
	};
}