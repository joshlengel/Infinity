#include"InfinityPCH.h"

#include"Shader.h"

namespace Infinity
{
	Shader::Shader(const VertexLayout &layout):
		m_vertex_layout(layout),
		m_constant_layout({})
	{}

	Shader::Shader(VertexLayout &&layout):
		m_vertex_layout(std::forward<VertexLayout>(layout)),
		m_constant_layout({})
	{}
}