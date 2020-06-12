#pragma once

#include"Core.h"

namespace Infinity
{
	class INFINITY_API Texture2D
	{
	public:
		Texture2D();
		virtual ~Texture2D();

		virtual bool Init(const char *filename, bool mipmap = false, float lod_bias = 0.0f) = 0;
		virtual bool Init(unsigned int width, unsigned int height, const unsigned char *pixels, bool mipmap = false, float lod_bias = 0.0f) = 0;
		virtual void Destroy() = 0;

		virtual void Bind(unsigned int slot) const = 0;

		static Texture2D *CreateTexture();
	};
}