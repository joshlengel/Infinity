#pragma once

#ifdef INFINITY_WINDOWS

#include"Model.h"

struct ID3D11Buffer;

namespace Infinity
{
	class WindowsVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_prev_size;
		ID3D11Buffer *m_buffer;

	public:
		WindowsVertexBuffer(const VertexLayout &layout);
		WindowsVertexBuffer(VertexLayout &&layout);
		~WindowsVertexBuffer();

		bool Init() override;
		void Destroy() override;

		bool SetData(const void *data, unsigned int size) override;
		void *GetNativeBuffer() const override;
	};

	class WindowsIndexBuffer : public IndexBuffer
	{
	private:
		unsigned int m_prev_size;
		unsigned int m_index_count;
		unsigned int m_index_size;
		ID3D11Buffer *m_buffer;

	public:
		WindowsIndexBuffer();
		~WindowsIndexBuffer();

		bool Init() override;
		void Destroy() override;

		bool SetData(const void *data, unsigned int size, unsigned int index_count) override;
		void *GetNativeBuffer() const override;

		unsigned int GetIndexCount() const override;
		unsigned int GetIndexSize() const override;
	};

	class INFINITY_API WindowsModel : public Model
	{
	public:
		WindowsModel(unsigned int num_vertex_buffers);
		~WindowsModel();

		void Bind() override;
		void Render() override;
	};
}

#endif // INFINITY_WINDOWS