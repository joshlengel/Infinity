#pragma once

#include"Core.h"

#include"Rasterizer.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API Context
	{
	protected:
		Resource<Rasterizer> m_def_rasterizer;

	public:
		virtual ~Context() {}

		virtual bool Init() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Context>;
}