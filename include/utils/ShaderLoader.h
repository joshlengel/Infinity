#pragma once

#include"Core.h"

#include"Shader.h"
#include"VertexLayout.h"

#include"utils/data/Map.h"
#include"utils/data/String.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API ShaderLoader
	{
	private:
		Map<String, Resource<Shader>> m_shaders;

	public:
		ShaderLoader();
		~ShaderLoader();

		Resource<Shader> Load(const String &name, const String &vertex_file, const String &pixel_file, const VertexLayout &layout);
		Resource<Shader> Load(String &&name, const String &vertex_file, const String &pixel_file, const VertexLayout &layout);
		Resource<Shader> Load(const String &name, const String &vertex_file, const String &pixel_file, VertexLayout &&layout);
		Resource<Shader> Load(String &&name, const String &vertex_file, const String &pixel_file, VertexLayout &&layout);

		Resource<Shader> Get(const String &name);

		void Remove(const String &name);
	};
}