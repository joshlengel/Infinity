#include"InfinityPCH.h"

#include"Model.h"

namespace Infinity
{
	VertexBuffer::VertexBuffer(const VertexLayout &layout):
		m_layout(layout)
	{}

	VertexBuffer::VertexBuffer(VertexLayout &&layout):
		m_layout(std::move(layout))
	{}

	VertexBuffer::~VertexBuffer()
	{}

	const VertexLayout &VertexBuffer::GetLayout() const { return m_layout; }

	IndexBuffer::IndexBuffer()
	{}

	IndexBuffer::~IndexBuffer()
	{}

	Model::Model(unsigned int num_vertex_buffers):
		m_num_vertex_buffers(num_vertex_buffers),
		m_vertex_buffers(new const VertexBuffer *[num_vertex_buffers]),
		m_index_buffer(nullptr)
	{}

	Model::~Model()
	{
		delete[] m_vertex_buffers;
	}

	void Model::SetVertexBuffer(unsigned int slot, const VertexBuffer *buffer) { m_vertex_buffers[slot] = buffer; }
	void Model::SetIndexBuffer(const IndexBuffer *buffer) { m_index_buffer = buffer; }
}