#pragma once

#include"Core.h"

#include"VertexLayout.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API VertexBuffer
	{
	protected:
		VertexLayout m_layout;

	public:
		VertexBuffer(const VertexLayout &layout);
		VertexBuffer(VertexLayout &&layout);
		virtual ~VertexBuffer() {}

		virtual bool Init(bool dynamic = false) = 0;

		virtual bool SetData(const void *data, unsigned int size) = 0;

		virtual void *GetNativeBuffer() const = 0;

		const VertexLayout &GetLayout() const;

		static Resource<VertexBuffer> CreateVertexBuffer(const VertexLayout &layout);
		static Resource<VertexBuffer> CreateVertexBuffer(VertexLayout &&layout);
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<VertexBuffer>;

	class INFINITY_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual bool Init(bool dynamic = false) = 0;

		virtual bool SetData(const void *data, unsigned int size, unsigned int index_count) = 0;
		virtual void *GetNativeBuffer() const = 0;

		virtual unsigned int GetIndexSize() const = 0;
		virtual unsigned int GetIndexCount() const = 0;

		static Resource<IndexBuffer> CreateIndexBuffer();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<IndexBuffer>;

	class INFINITY_API Model
	{
	protected:
		ArrayList<Resource<VertexBuffer>> m_vertex_buffers;
		Resource<IndexBuffer> m_index_buffer;

	public:
		Model(unsigned int num_vertex_buffers);
		virtual ~Model() {}

		void SetVertexBuffer(unsigned int slot, Resource<VertexBuffer> buffer);
		void SetIndexBuffer(Resource<IndexBuffer> buffer);

		virtual void Bind() = 0;
		virtual void Render() = 0;
		virtual void Render(unsigned int index_count) = 0;

		static Resource<Model> CreateModel(unsigned int num_vertex_buffers);
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Model>;
}