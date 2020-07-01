#pragma once

#include"Core.h"

#include"utils/data/String.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API Texture2D
	{
	public:
		virtual ~Texture2D() {}

		virtual bool Init(const String &filename, bool mipmap = false, float lod_bias = 0.0f) = 0;
		virtual bool Init(unsigned int width, unsigned int height, const unsigned char *pixels, bool mipmap = false, float lod_bias = 0.0f) = 0;
		
		virtual void Bind(unsigned int slot) const = 0;

		static Resource<Texture2D> CreateTexture();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Texture2D>;
}