#pragma once

#include"Core.h"

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
		Rasterizer();
		virtual ~Rasterizer();

		virtual bool Init(CullMode cull = CullMode::NONE, bool blend = false) = 0;
		virtual void Destroy() = 0;

		virtual void Bind() = 0;

		static Rasterizer *CreateRasterizer();
	};
}