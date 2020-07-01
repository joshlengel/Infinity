#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsTexture.h"
#include"platform/windows/WindowsWindow.h"
#include"platform/windows/WindowsContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

namespace Infinity
{
	Resource<Texture2D> Texture2D::CreateTexture()
	{
		return ResourceCast<Texture2D>(MakeResource<WindowsTexture2D>());
	}

	WindowsTexture2D::WindowsTexture2D():
		m_width(),
		m_height(),
		m_pixels(),
		m_texture(nullptr),
		m_shader_resource_view(nullptr),
		m_sampler_state(nullptr)
	{}

	WindowsTexture2D::~WindowsTexture2D()
	{
		if (m_sampler_state)
		{
			m_sampler_state->Release();
		}

		if (m_shader_resource_view)
		{
			m_shader_resource_view->Release();
		}

		if (m_texture)
		{
			m_texture->Release();
		}

		if (m_pixels)
		{
			free(m_pixels); // must use free due to stbimage
		}
	}

	bool WindowsTexture2D::Init(const String &filename, bool mipmap, float lod_bias)
	{
		int width, height, cif;

		m_pixels = stbi_load(&filename[0], &width, &height, &cif, STBI_rgb_alpha);

		m_width = (unsigned)width;
		m_height = (unsigned)height;

		return InitDirect3D(mipmap, lod_bias);
	}

	bool WindowsTexture2D::Init(unsigned int width, unsigned int height, const unsigned char *pixels, bool mipmap, float lod_bias)
	{
		m_width = width;
		m_height = height;

		unsigned int size = width * height * 4;
		m_pixels = (unsigned char*)malloc(size); // must use malloc to be consistent with stbimage
		memcpy(m_pixels, pixels, size);

		return InitDirect3D(mipmap, lod_bias);
	}

	bool WindowsTexture2D::InitDirect3D(bool mipmap, float load_bias)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11Device *device = context->GetDevice();
		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC tex_desc;
		tex_desc.Width = m_width;
		tex_desc.Height = m_height;
		tex_desc.MipLevels = mipmap? 0 : 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = mipmap? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		if (FAILED(device->CreateTexture2D(&tex_desc, nullptr, &m_texture)))
		{
			INFINITY_CORE_ERROR("Error creating native texture");
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = tex_desc.Format;
		srv_desc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = mipmap? (unsigned int)-1 : 1;

		if (FAILED(device->CreateShaderResourceView(m_texture, &srv_desc, &m_shader_resource_view)))
		{
			INFINITY_CORE_ERROR("Error creating native texture");
			return false;
		}

		device_context->UpdateSubresource(m_texture, 0, nullptr, m_pixels, 4 * m_width * sizeof(unsigned char), 0);

		if (mipmap)
			device_context->GenerateMips(m_shader_resource_view);

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.MipLODBias = mipmap? load_bias : 0.0f;
		sampler_desc.MinLOD = -D3D11_FLOAT32_MAX;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		sampler_desc.MaxAnisotropy = 1;

		if (FAILED(device->CreateSamplerState(&sampler_desc, &m_sampler_state)))
		{
			INFINITY_CORE_ERROR("Error creating native texture");
			return false;
		}

		return true;
	}
	
	void WindowsTexture2D::Bind(unsigned int slot) const
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->PSSetShaderResources(slot, 1, &m_shader_resource_view);
		device_context->PSSetSamplers(slot, 1, &m_sampler_state);
	}
}

#endif // INFINITY_WINDOWS
