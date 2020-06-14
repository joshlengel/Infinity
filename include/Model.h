#pragma once

#include"Core.h"

#include"VertexLayout.h"

namespace Infinity
{
	class INFINITY_API VertexBuffer
	{
	protected:
		VertexLayout m_layout;

	public:
		VertexBuffer(const VertexLayout &layout);
		VertexBuffer(VertexLayout &&layout);
		virtual ~VertexBuffer();

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual bool SetData(const void *data, unsigned int size) = 0;

		virtual void *GetNativeBuffer() const = 0;

		const VertexLayout &GetLayout() const;

		static VertexBuffer *CreateVertexBuffer(const VertexLayout &layout);
		static VertexBuffer *CreateVertexBuffer(VertexLayout &&layout);
	};

	class INFINITY_API IndexBuffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual bool SetData(const void *data, unsigned int size, unsigned int index_count) = 0;
		virtual void *GetNativeBuffer() const = 0;

		virtual unsigned int GetIndexSize() const = 0;
		virtual unsigned int GetIndexCount() const = 0;

		static IndexBuffer *CreateIndexBuffer();
	};

	class INFINITY_API Model
	{
	protected:
		unsigned int m_num_vertex_buffers;
		const VertexBuffer **m_vertex_buffers;
		const IndexBuffer *m_index_buffer;

	public:
		Model(unsigned int num_vertex_buffers);
		virtual ~Model();

		void SetVertexBuffer(unsigned int slot, const VertexBuffer *buffer);
		void SetIndexBuffer(const IndexBuffer *buffer);

		virtual void Bind() = 0;
		virtual void Render() = 0;

		static Model *CreateModel(unsigned int num_vertex_buffers);
	};
}