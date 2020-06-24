#pragma once

#include"Core.h"

#include"utils/data/Map.h"
#include"utils/data/String.h"

namespace Infinity
{
	class Shader;
	class VertexLayout;

	class INFINITY_API ShaderLoader
	{
	private:
		Map<StaticString, Shader*> m_shaders;

	public:
		ShaderLoader();
		~ShaderLoader();

		void Destroy();

		Shader *Load(StaticString name, const char *vertex_file, const char *pixel_file, const VertexLayout &layout);
		Shader *Load(StaticString name, const char *vertex_file, const char *pixel_file, VertexLayout &&layout);

		Shader *Get(StaticString name);

		void Destroy(StaticString name);
	};
}