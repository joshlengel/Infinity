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
	{
		m_shaders.Clear();
	}

	Resource<Shader> ShaderLoader::Load(const String &name, const String &vertex_file, const String &pixel_file, const VertexLayout &layout)
	{
		std::ifstream vertex_f(&vertex_file[0]);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &vertex_file[0]);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(&pixel_file[0]);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &pixel_file[0]);
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

		Resource<Shader> shader = Shader::CreateShader(layout);

		if (!shader->Init(vertex, pixel))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[name] = shader;

		return shader;
	}

	Resource<Shader> ShaderLoader::Load(String &&name, const String &vertex_file, const String &pixel_file, const VertexLayout &layout)
	{
		std::ifstream vertex_f(&vertex_file[0]);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &vertex_file[0]);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(&pixel_file[0]);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &pixel_file[0]);
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

		Resource<Shader> shader = Shader::CreateShader(layout);

		if (!shader->Init(vertex, pixel))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[std::forward<String>(name)] = shader;

		return shader;
	}

	Resource<Shader> ShaderLoader::Load(const String &name, const String &vertex_file, const String &pixel_file, VertexLayout &&layout)
	{
		std::ifstream vertex_f(&vertex_file[0]);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &vertex_file[0]);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(&pixel_file[0]);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &pixel_file[0]);
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

		Resource<Shader> shader = Shader::CreateShader(std::forward<VertexLayout>(layout));

		if (!shader->Init(vertex, pixel))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[name] = shader;

		return shader;
	}

	Resource<Shader> ShaderLoader::Load(String &&name, const String &vertex_file, const String &pixel_file, VertexLayout &&layout)
	{
		std::ifstream vertex_f(&vertex_file[0]);

		if (!vertex_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &vertex_file[0]);
			return nullptr;
		}

		std::stringstream vertex_ss;
		std::string line;

		while (std::getline(vertex_f, line))
		{
			vertex_ss << line << '\n';
		}

		std::ifstream pixel_f(&pixel_file[0]);

		if (!pixel_f)
		{
			INFINITY_CORE_ERROR("Error finding shader file '{0}'", &pixel_file[0]);
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

		Resource<Shader> shader = Shader::CreateShader(std::forward<VertexLayout>(layout));

		if (!shader->Init(vertex, pixel))
		{
			INFINITY_CORE_ERROR("Error initializing shader in shader loader");
		}

		m_shaders[std::forward<String>(name)] = shader;

		return shader;
	}

	Resource<Shader> ShaderLoader::Get(const String &name)
	{
		auto itr = Find(m_shaders, name);
		
		return itr == m_shaders.end()? nullptr : itr->value;
	}

	void ShaderLoader::Remove(const String &name)
	{
		Infinity::Remove(m_shaders, name);
	}
}