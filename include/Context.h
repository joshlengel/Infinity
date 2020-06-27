#pragma once

#include"Core.h"

namespace Infinity
{
	class Rasterizer;

	class INFINITY_API Context
	{
	protected:
		Rasterizer *m_def_rasterizer;

	public:
		Context();
		virtual ~Context();

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
	};
}