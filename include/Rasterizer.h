#pragma once

#include"Core.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API Rasterizer
	{
	public:
		enum class CullMode : unsigned char
		{
			CLOCKWISE,
			COUNTERCLOCKWISE,
			NONE
		};

	public:
		virtual ~Rasterizer() {}

		virtual bool Init(CullMode cull = CullMode::NONE, bool blend = false) = 0;
		
		virtual void Bind() = 0;

		static Resource<Rasterizer> CreateRasterizer();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Rasterizer>;
}