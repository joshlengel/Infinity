#include"InfinityPCH.h"

#include"Model.h"

namespace Infinity
{
	VertexBuffer::VertexBuffer(const VertexLayout &layout):
		m_layout(layout)
	{}

	VertexBuffer::VertexBuffer(VertexLayout &&layout):
		m_layout(std::forward<VertexLayout>(layout))
	{}

	const VertexLayout &VertexBuffer::GetLayout() const { return m_layout; }

	Model::Model(unsigned int num_vertex_buffers):
		m_vertex_buffers(num_vertex_buffers),
		m_index_buffer(nullptr)
	{
		m_vertex_buffers.Expand(num_vertex_buffers);
	}

	void Model::SetVertexBuffer(unsigned int slot, Resource<VertexBuffer> buffer) { m_vertex_buffers[slot] = buffer; }
	void Model::SetIndexBuffer(Resource<IndexBuffer> buffer) { m_index_buffer = buffer; }
}