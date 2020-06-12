#ifdef INFINITY_WINDOWS

#pragma once

#include"Core.h"
#include"Texture.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace Infinity
{
	class INFINITY_API WindowsTexture2D : public Texture2D
	{
	private:
		unsigned int m_width, m_height;
		unsigned char *m_pixels;
		ID3D11Texture2D *m_texture;
		ID3D11ShaderResourceView *m_shader_resource_view;
		ID3D11SamplerState *m_sampler_state;

	public:
		WindowsTexture2D();
		~WindowsTexture2D();

		bool Init(const char *filename, bool mipmap = false, float lod_bias = 0.0f) override;
		bool Init(unsigned int width, unsigned int height, const unsigned char *pixels, bool mipmap = false, float lod_bias = 0.0f) override;
		void Destroy() override;

		void Bind(unsigned int slot) const override;

	private:
		bool InitDirect3D(bool mipmap, float lod_bias);
	};
}

#endif // INFINITY_WINDOWS