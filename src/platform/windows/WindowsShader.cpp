#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsShader.h"

#include"platform/windows/WindowsWindow.h"
#include"platform/windows/WindowsContext.h"

namespace Infinity
{
	constexpr DXGI_FORMAT GetFormat(DataType type)
	{
		switch (type)
		{
		case DataType::INT: return DXGI_FORMAT_R32_SINT;
		case DataType::INT2: return DXGI_FORMAT_R32G32_SINT;
		case DataType::INT3: return DXGI_FORMAT_R32G32B32_SINT;
		case DataType::INT4: return DXGI_FORMAT_R32G32B32A32_SINT;

		case DataType::FLOAT: return DXGI_FORMAT_R32_FLOAT;
		case DataType::FLOAT2: return DXGI_FORMAT_R32G32_FLOAT;
		case DataType::FLOAT3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case DataType::FLOAT4: return DXGI_FORMAT_R32G32B32A32_FLOAT;

		default: return DXGI_FORMAT_UNKNOWN;
		}
	}

	Shader *Shader::CreateShader(const VertexLayout &layout)
	{
		return new WindowsShader(layout);
	}

	Shader *Shader::CreateShader(VertexLayout &&layout)
	{
		return new WindowsShader(std::move(layout));
	}

	WindowsShader::WindowsShader(const VertexLayout &layout):
		Shader(layout),
		m_vertex_shader(nullptr),
		m_pixel_shader(nullptr),
		m_input_layout(nullptr),
		m_constant_buffer(nullptr),
		m_constant_subres()
	{}

	WindowsShader::WindowsShader(VertexLayout &&layout):
		Shader(std::move(layout)),
		m_vertex_shader(nullptr),
		m_pixel_shader(nullptr),
		m_input_layout(nullptr),
		m_constant_buffer(nullptr),
		m_constant_subres()
	{}

	WindowsShader::~WindowsShader()
	{}

	bool WindowsShader::Init(const char *vertex_source, unsigned int vs_size, const char *pixel_source, unsigned int ps_size)
	{
		ID3DBlob *vertex_bc, *pixel_bc, *error = nullptr;
		
		if (FAILED(D3DCompile(vertex_source, vs_size, "vertex", nullptr, nullptr, "main", "vs_5_0",
			0, 0, &vertex_bc, &error)))
		{
			if (!error)
			{
				INFINITY_CORE_ERROR("Error compiling vertex shader. No error message");
				return false;
			}
			else
			{
				INFINITY_CORE_ERROR("Error compiling vertex shader. Error log:\n{0}", (char*)error->GetBufferPointer());
				error->Release();
				return false;
			}
		}

		if (FAILED(D3DCompile(pixel_source, ps_size, "pixel", nullptr, nullptr, "main", "ps_5_0",
			0, 0, &pixel_bc, &error)))
		{
			if (!error)
			{
				INFINITY_CORE_ERROR("Error compiling pixel shader. No error message");
				return false;
			}
			else
			{
				INFINITY_CORE_ERROR("Error compiling pixel shader. Error log:\n{0}", (char*)error->GetBufferPointer());
				error->Release();
				return false;
			}
		}

		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11Device *device = context->GetDevice();
		
		if (FAILED(device->CreateVertexShader(vertex_bc->GetBufferPointer(), vertex_bc->GetBufferSize(), nullptr, &m_vertex_shader)))
		{
			vertex_bc->Release();
			pixel_bc->Release();
			return false;
		}

		if (FAILED(device->CreatePixelShader(pixel_bc->GetBufferPointer(), pixel_bc->GetBufferSize(), nullptr, &m_pixel_shader)))
		{
			vertex_bc->Release();
			pixel_bc->Release();
			return false;
		}

		pixel_bc->Release();

		unsigned int num_elements = m_vertex_layout.GetNumElements();
		D3D11_INPUT_ELEMENT_DESC *elements = new D3D11_INPUT_ELEMENT_DESC[num_elements];

		const VertexLayout::Element *itr = m_vertex_layout.begin();

		for (unsigned int i = 0; i < num_elements; ++i)
		{
			D3D11_INPUT_ELEMENT_DESC &desc = elements[i];

			desc.SemanticName = "ATTRIBUTE";
			desc.SemanticIndex = i;
			desc.InputSlot = itr->input_slot;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.AlignedByteOffset = itr->offset;
			desc.Format = GetFormat(itr->type);
			desc.InstanceDataStepRate = 0;

			++itr;
		}

		if (FAILED(device->CreateInputLayout(elements, m_vertex_layout.GetNumElements(), vertex_bc->GetBufferPointer(), vertex_bc->GetBufferSize(), &m_input_layout)))
		{
			INFINITY_CORE_ERROR("Error creating shader input layout");
			vertex_bc->Release();
			delete[] elements;
			return false;
		}

		delete[] elements;
		vertex_bc->Release();

		return true;
	}

	void WindowsShader::Destroy()
	{
		if (m_vertex_shader)
		{
			m_vertex_shader->Release();
			m_vertex_shader = nullptr;
		}

		if (m_pixel_shader)
		{
			m_pixel_shader->Release();
			m_pixel_shader = nullptr;
		}

		if (m_input_layout)
		{
			m_input_layout->Release();
			m_input_layout = nullptr;
		}

		if (m_constant_buffer)
		{
			m_constant_buffer->Release();
			m_constant_buffer = nullptr;
		}
	}

	bool WindowsShader::DeclareConstants(const VertexLayout &layout)
	{
		m_constant_layout = layout;

		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11Device *device = context->GetDevice();

		if (m_constant_buffer)
		{
			m_constant_buffer->Release();
			m_constant_buffer = nullptr;
		}

		unsigned int bytes = 0;

		for (auto &elem : m_constant_layout)
		{
			switch (elem.type)
			{
			case DataType::MAT3: bytes += 12 * 4; break;
			case DataType::MAT4: bytes += 16 * 4; break;
			default: bytes += 4 * 4; break;
			}
		}

		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.ByteWidth = bytes;

		if (FAILED(device->CreateBuffer(&buffer_desc, nullptr, &m_constant_buffer)))
		{
			INFINITY_CORE_ERROR("Error creating constant buffer for shader");
			return false;
		}

		return true;
	}

	bool WindowsShader::DeclareConstants(VertexLayout &&layout)
	{
		m_constant_layout = std::move(layout);

		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11Device *device = context->GetDevice();

		if (m_constant_buffer)
		{
			m_constant_buffer->Release();
			m_constant_buffer = nullptr;
		}

		unsigned int bytes = 0;

		for (auto &elem : m_constant_layout)
		{
			switch (elem.type)
			{
			case DataType::MAT3: bytes += 12 * 4; break;
			case DataType::MAT4: bytes += 16 * 4; break;
			default: bytes += 4 * 4; break;
			}
		}

		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.ByteWidth = bytes;

		if (FAILED(device->CreateBuffer(&buffer_desc, nullptr, &m_constant_buffer)))
		{
			INFINITY_CORE_ERROR("Error creating constant buffer for shader");
			return false;
		}

		return true;
	}

	int WindowsShader::GetConstantLocation(const String &name)
	{
		unsigned int bytes = 0;

		for (auto &elem : m_constant_layout)
		{
			if (elem.name == name) return (signed)bytes;

			switch (elem.type)
			{
			case DataType::MAT3: bytes += 12 * 4; break;
			case DataType::MAT4: bytes += 16 * 4; break;
			default: bytes += 4 * 4; break;
			}
		}

		return -1;
	}

	bool WindowsShader::MapConstants()
	{
		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		if (FAILED(device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_constant_subres)))
		{
			INFINITY_CORE_ERROR("Error loading constant buffer from vram");
			return false;
		}

		return true;
	}

	void WindowsShader::UnmapConstants()
	{
		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->Unmap(m_constant_buffer, 0);

		m_constant_subres.pData = nullptr;
	}

	void WindowsShader::Bind()
	{
		WindowsContext *context = (WindowsContext*)Window::GetContext();

		ID3D11DeviceContext *device_context = context->GetDeviceContext();

		device_context->VSSetShader(m_vertex_shader, nullptr, 0);
		device_context->PSSetShader(m_pixel_shader, nullptr, 0);

		device_context->IASetInputLayout(m_input_layout);

		device_context->VSSetConstantBuffers(0, 1, &m_constant_buffer);
	}

	// SetConstant*

	bool WindowsShader::AssureValid(int location)
	{
		if (!m_constant_subres.pData)
		{
			INFINITY_CORE_WARN("Must call MapConstants before invoking SetConstant");
			return false;
		}

		if (location == -1)
		{
			INFINITY_CORE_WARN("Invalid location passed into SetConstant*()");
			return false;
		}

		return true;
	}

	void WindowsShader::SetConstant1i(int location, int i)
	{
		if (!AssureValid(location)) return;

		memcpy((char*)m_constant_subres.pData + location, &i, sizeof(int));
	}

	void WindowsShader::SetConstant2i(int location, int i1, int i2)
	{
		if (!AssureValid(location)) return;

		int data[2] = { i1, i2 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant3i(int location, int i1, int i2, int i3)
	{
		if (!AssureValid(location)) return;

		int data[3] = { i1, i2, i3 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant4i(int location, int i1, int i2, int i3, int i4)
	{
		if (!AssureValid(location)) return;

		int data[4] = { i1, i2, i3, i4 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant1f(int location, float f)
	{
		if (!AssureValid(location)) return;

		memcpy((char*)m_constant_subres.pData + location, &f, sizeof(float));
	}

	void WindowsShader::SetConstant2f(int location, float f1, float f2)
	{
		if (!AssureValid(location)) return;

		float data[2] = { f1, f2 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant3f(int location, float f1, float f2, float f3)
	{
		if (!AssureValid(location)) return;

		float data[3] = { f1, f2, f3 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant4f(int location, float f1, float f2, float f3, float f4)
	{
		if (!AssureValid(location)) return;

		float data[4] = { f1, f2, f3, f4 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant1d(int location, double d)
	{
		if (!AssureValid(location)) return;

		memcpy((char*)m_constant_subres.pData + location, &d, sizeof(double));
	}

	void WindowsShader::SetConstant2d(int location, double d1, double d2)
	{
		if (!AssureValid(location)) return;

		double data[2] = { d1, d2 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant3d(int location, double d1, double d2, double d3)
	{
		if (!AssureValid(location)) return;

		double data[3] = { d1, d2, d3 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstant4d(int location, double d1, double d2, double d3, double d4)
	{
		if (!AssureValid(location)) return;

		double data[4] = { d1, d2, d3, d4 };

		memcpy((char*)m_constant_subres.pData + location, data, sizeof(data));
	}

	void WindowsShader::SetConstantMat3(int location, const float *matrix, bool transpose)
	{
		if (!AssureValid(location)) return;

		const float m_transp[9] =
		{
			matrix[0], matrix[3], matrix[6],
			matrix[1], matrix[4], matrix[7],
			matrix[2], matrix[5], matrix[8]
		};

		const float *m;

		if (transpose)
		{
			m = m_transp;
		}
		else
		{
			m = matrix;
		}

		memcpy((char*)m_constant_subres.pData + location, m, sizeof(m_transp));
	}

	void WindowsShader::SetConstantMat4(int location, const float *matrix, bool transpose)
	{
		if (!AssureValid(location)) return;

		const float m_transp[16] =
		{
			matrix[0], matrix[4], matrix[8], matrix[12],
			matrix[1], matrix[5], matrix[9], matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15]
		};

		const float *m;

		if (transpose)
		{
			m = m_transp;
		}
		else
		{
			m = matrix;
		}

		memcpy((char*)m_constant_subres.pData + location, m, sizeof(m_transp));
	}
}

#endif // INFINITY_WINDOWS