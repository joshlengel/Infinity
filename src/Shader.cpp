#include"Shader.h"

namespace Infinity
{
	Shader::Shader(const VertexLayout &layout):
		m_vertex_layout(layout),
		m_constant_layout({})
	{}

	Shader::Shader(VertexLayout &&layout):
		m_vertex_layout(std::move(layout)),
		m_constant_layout({})
	{}

	Shader::~Shader()
	{}
}