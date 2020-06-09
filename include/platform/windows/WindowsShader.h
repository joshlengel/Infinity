#pragma once

#ifdef INFINITY_WINDOWS

#pragma comment(lib, "d3dcompiler.lib")

#include "Core.h"

#include"Shader.h"

#include<d3d11.h>

namespace Infinity
{
	class WindowsShader : public Shader
	{
	private:
		ID3D11VertexShader *m_vertex_shader;
		ID3D11PixelShader *m_pixel_shader;
		ID3D11InputLayout *m_input_layout;

		ID3D11Buffer *m_constant_buffer;
		D3D11_MAPPED_SUBRESOURCE m_constant_subres;

	public:
		WindowsShader(const VertexLayout &layout);
		WindowsShader(VertexLayout &&layout);
		~WindowsShader();

		bool Init(const char *vertex_source, unsigned int vs_size, const char *pixel_source, unsigned int ps_size) override;
		void Destroy() override;

		bool DeclareConstants(const VertexLayout &layout) override;
		bool DeclareConstants(VertexLayout &&layout) override;

		int GetConstantLocation(const std::string &name) override;
		
		bool MapConstants() override;
		void UnmapConstants() override;

		void SetConstant1i(int location, int i) override;
		void SetConstant2i(int location, int i1, int i2) override;
		void SetConstant3i(int location, int i1, int i2, int i3) override;
		void SetConstant4i(int location, int i1, int i2, int i3, int i4) override;
		void SetConstant1f(int location, float f) override;
		void SetConstant2f(int location, float f1, float f2) override;
		void SetConstant3f(int location, float f1, float f2, float f3) override;
		void SetConstant4f(int location, float f1, float f2, float f3, float f4) override;
		void SetConstant1d(int location, double d) override;
		void SetConstant2d(int location, double d1, double d2) override;
		void SetConstant3d(int location, double d1, double d2, double d3) override;
		void SetConstant4d(int location, double d1, double d2, double d3, double d4) override;
		void SetConstantMat3(int location, const float *matrix, bool transpose) override;
		void SetConstantMat4(int location, const float *matrix, bool transpose) override;
	
		void Bind() override;

	private:
		bool AssureValid(int location);
	};
}

#endif // INFINITY_WINDOWS