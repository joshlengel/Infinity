#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsModel.h"

#include"platform/windows/WindowsWindow.h"
#include"platform/windows/WindowsContext.h"

constexpr DXGI_FORMAT GetFormat(unsigned int bytes)
{
	switch (bytes)
	{
	case 1: return DXGI_FORMAT_R8_UINT;
	case 2: return DXGI_FORMAT_R16_UINT;
	case 4: return DXGI_FORMAT_R32_UINT;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

namespace Infinity
{
	Resource<VertexBuffer> VertexBuffer::CreateVertexBuffer(const VertexLayout &layout)
	{
		return ResourceCast<VertexBuffer>(MakeResource<WindowsVertexBuffer>(layout));
	}

	Resource<VertexBuffer> VertexBuffer::CreateVertexBuffer(VertexLayout &&layout)
	{
		return ResourceCast<VertexBuffer>(MakeResource<WindowsVertexBuffer>(std::forward<VertexLayout>(layout)));
	}

	Resource<IndexBuffer> IndexBuffer::CreateIndexBuffer()
	{
		return ResourceCast<IndexBuffer>(MakeResource<WindowsIndexBuffer>());
	}

	Resource<Model> Model::CreateModel(unsigned int num_vertex_buffers)
	{
		return ResourceCast<Model>(MakeResource<WindowsModel>(num_vertex_buffers));
	}

	WindowsVertexBuffer::WindowsVertexBuffer(const VertexLayout &layout):
		VertexBuffer(layout),
		m_buffer(nullptr),
		m_prev_size(),
		m_dynamic()
	{}

	WindowsVertexBuffer::WindowsVertexBuffer(VertexLayout &&layout):
		VertexBuffer(std::forward<VertexLayout>(layout)),
		m_buffer(nullptr),
		m_prev_size(),
		m_dynamic()
	{}

	WindowsVertexBuffer::~WindowsVertexBuffer()
	{
		if (m_buffer)
		{
			m_buffer->Release();
		}
	}

	bool WindowsVertexBuffer::Init(bool dynamic)
	{
		m_dynamic = dynamic;
		return true;
	}

	bool WindowsVertexBuffer::SetData(const void *data, unsigned int size)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());
		
		ID3D11Device *device = context->GetDevice();
		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		if (size <= m_prev_size)
		{
			D3D11_MAPPED_SUBRESOURCE mapped_subres;

			if (FAILED(device_context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subres))) return false;

			if (data)
				memcpy(mapped_subres.pData, data, size);
			else
				memset(mapped_subres.pData, 0, size);

			device_context->Unmap(m_buffer, 0);
		}
		else
		{
			if (m_buffer) m_buffer->Release();
			
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.Usage = m_dynamic? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			buffer_desc.ByteWidth = size;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = m_dynamic? D3D11_CPU_ACCESS_WRITE : 0;

			D3D11_SUBRESOURCE_DATA subres_data = {};
			subres_data.pSysMem = data;
			subres_data.SysMemPitch = size;

			if (FAILED(device->CreateBuffer(&buffer_desc, data? &subres_data : nullptr, &m_buffer))) return false;
			
			m_prev_size = size;
		}

		return true;
	}

	void *WindowsVertexBuffer::GetNativeBuffer() const { return m_buffer; }

	WindowsIndexBuffer::WindowsIndexBuffer():
		m_buffer(nullptr),
		m_prev_size(),
		m_index_count(),
		m_index_size(),
		m_dynamic()
	{}

	WindowsIndexBuffer::~WindowsIndexBuffer()
	{
		if (m_buffer)
		{
			m_buffer->Release();
		}
	}

	bool WindowsIndexBuffer::Init(bool dynamic)
	{
		m_dynamic = dynamic;
		return true;
	}

	bool WindowsIndexBuffer::SetData(const void *data, unsigned int size, unsigned int index_count)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11Device *device = context->GetDevice();
		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		m_index_count = index_count;

		if (size <= m_prev_size)
		{
			D3D11_MAPPED_SUBRESOURCE mapped_subres;

			if (FAILED(device_context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subres))) return false;

			if (data)
				memcpy(mapped_subres.pData, data, size);
			else
				memset(mapped_subres.pData, 0, size);

			device_context->Unmap(m_buffer, 0);
		}
		else
		{
			if (m_buffer) m_buffer->Release();

			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.Usage = m_dynamic? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			buffer_desc.ByteWidth = size;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = m_dynamic? D3D11_CPU_ACCESS_WRITE : 0;

			D3D11_SUBRESOURCE_DATA subres_data = {};
			subres_data.pSysMem = data;
			subres_data.SysMemPitch = size;

			if (FAILED(device->CreateBuffer(&buffer_desc, data? &subres_data : nullptr, &m_buffer))) return false;

			m_prev_size = size;
		}

		m_index_size = size / index_count;

		return true;
	}

	void *WindowsIndexBuffer::GetNativeBuffer() const { return m_buffer; }

	unsigned int WindowsIndexBuffer::GetIndexCount() const { return m_index_count; }
	unsigned int WindowsIndexBuffer::GetIndexSize() const { return m_index_size; }

	WindowsModel::WindowsModel(unsigned int num_vertex_buffers):
		Model(num_vertex_buffers)
	{}

	WindowsModel::~WindowsModel()
	{}

	void WindowsModel::Bind()
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		unsigned int index = 0;
		for (const Resource<VertexBuffer> &vertex_buffer : m_vertex_buffers)
		{
			ID3D11Buffer *buffer = (ID3D11Buffer*)vertex_buffer->GetNativeBuffer();
			unsigned int stride = vertex_buffer->GetLayout().GetStride();
			unsigned int offset = 0;

			device_context->IASetVertexBuffers(index++, 1, &buffer, &stride, &offset);
		}

		device_context->IASetIndexBuffer((ID3D11Buffer*)m_index_buffer->GetNativeBuffer(), GetFormat(m_index_buffer->GetIndexSize()), 0);
		
		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void WindowsModel::Render()
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->DrawIndexed(m_index_buffer->GetIndexCount(), 0, 0);
	}

	void WindowsModel::Render(unsigned int index_count)
	{
		Resource<WindowsContext> context = ResourceCast<WindowsContext>(Window::GetContext());

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->DrawIndexed(index_count, 0, 0);
	}
}

#endif // INFINITY_WINDOWS
