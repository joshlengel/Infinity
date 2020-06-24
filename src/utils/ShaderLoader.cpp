#include"InfinityPCH.h"

#include"utils/ShaderLoader.h"

#include"VertexLayout.h"
#include"Shader.h"

namespace Infinity
{
	ShaderLoader::ShaderLoader():
		m_shaders()
	{}

	ShaderLoader::~ShaderLoader()
	{}

	void ShaderLoader::Destroy()
	{
		for (auto &entry : m_shaders)
		{
			Shader *shader = entry.value;

			if (shader)
			{
				shader->Destroy();
				delete shader;
			}
		}

		m_shaders.Clear();
	}

	Shader *ShaderLoader::Load(StaticString name, const char *vertex_file, const char *pixel_file, const VertexLayout &layout)
	{
		std::ifstream vertex_f(vertex_file);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", vertex_file);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(pixel_file);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", pixel_file);
			return nullptr;
		}

		std::stringstream pixel_ss;
		
		while (std::getline(pixel_f, line))
		{
			pixel_ss << line << '\n';
		}

		std::string vertex_src = vertex_ss.str();
		std::string pixel_src = pixel_ss.str();

		const char *vertex = vertex_src.c_str();
		const char *pixel = pixel_src.c_str();

		unsigned int vertex_size = (unsigned int)(vertex_src.length() * sizeof(char));
		unsigned int pixel_size = (unsigned int)(pixel_src.length() * sizeof(char));

		Shader *shader = Shader::CreateShader(layout);

		if (!shader->Init(vertex, vertex_size, pixel, pixel_size))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[name] = shader;

		return shader;
	}

	Shader *ShaderLoader::Load(StaticString name, const char *vertex_file, const char *pixel_file, VertexLayout &&layout)
	{
		std::ifstream vertex_f(vertex_file);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", vertex_file);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(pixel_file);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", pixel_file);
			return nullptr;
		}

		std::stringstream pixel_ss;

		while (std::getline(pixel_f, line))
		{
			pixel_ss << line << '\n';
		}

		std::string vertex_src = vertex_ss.str();
		std::string pixel_src = pixel_ss.str();

		const char *vertex = vertex_src.c_str();
		const char *pixel = pixel_src.c_str();

		unsigned int vertex_size = (unsigned int)(vertex_src.length() * sizeof(char));
		unsigned int pixel_size = (unsigned int)(pixel_src.length() * sizeof(char));

		Shader *shader = Shader::CreateShader(std::move(layout));

		if (!shader->Init(vertex, vertex_size, pixel, pixel_size))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[name] = shader;

		return shader;
	}

	Shader *ShaderLoader::Get(StaticString name)
	{
		return m_shaders[name];
	}

	void ShaderLoader::Destroy(StaticString name)
	{
		Shader *shader = m_shaders[name];

		if (shader)
		{
			shader->Destroy();
			delete shader;
		}

		m_shaders.Remove(name);
	}
}