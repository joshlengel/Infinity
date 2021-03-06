#pragma once

#include"Core.h"

#include"VertexLayout.h"

#include"utils/data/String.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API Shader
	{
	protected:
		VertexLayout m_vertex_layout;
		VertexLayout m_constant_layout;

	public:
		Shader(const VertexLayout &layout);
		Shader(VertexLayout &&layout);
		virtual ~Shader() {}

		virtual bool Init(const String &vertex_source, const String &pixel_source) = 0;
		
		virtual bool DeclareConstants(const VertexLayout &layout) = 0;
		virtual bool DeclareConstants(VertexLayout &&layout) = 0;

		virtual int GetConstantLocation(const String &name) = 0;

		virtual bool MapConstants() = 0;
		virtual void UnmapConstants() = 0;

		virtual void SetConstant1i(int location, int i) = 0;
		virtual void SetConstant2i(int location, int i1, int i2) = 0;
		virtual void SetConstant3i(int location, int i1, int i2, int i3) = 0;
		virtual void SetConstant4i(int location, int i1, int i2, int i3, int i4) = 0;
		virtual void SetConstant1f(int location, float f) = 0;
		virtual void SetConstant2f(int location, float f1, float f2) = 0;
		virtual void SetConstant3f(int location, float f1, float f2, float f3) = 0;
		virtual void SetConstant4f(int location, float f1, float f2, float f3, float f4) = 0;
		virtual void SetConstant1d(int location, double d) = 0;
		virtual void SetConstant2d(int location, double d1, double d2) = 0;
		virtual void SetConstant3d(int location, double d1, double d2, double d3) = 0;
		virtual void SetConstant4d(int location, double d1, double d2, double d3, double d4) = 0;
		virtual void SetConstantMat3(int location, const float *matrix, bool transpose) = 0;
		virtual void SetConstantMat4(int location, const float *matrix, bool transpose) = 0;

		virtual void Bind() = 0;
		
		static Resource<Shader> CreateShader(const VertexLayout &layout);
		static Resource<Shader> CreateShader(VertexLayout &&layout);
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Shader>;
}