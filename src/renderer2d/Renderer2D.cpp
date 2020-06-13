#include"renderer2d/Renderer2D.h"

#include"Model.h"
#include"Shader.h"
#include"Type.h"

#include"Texture.h"

#include"Log.h"

constexpr static const char v_shader_source[] = R"(
cbuffer Constants
{
	matrix model;
	matrix projection_view;
	
	float4 color;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

VertexOut main(float2 vertex : ATTRIBUTE0)
{
	VertexOut output;
	
	output.position = mul(float4(vertex, 0.0, 1.0), model);
	output.position = mul(output.position, projection_view);
	output.texcoord.x = (vertex.x + 1) * 0.5;
	output.texcoord.y = (1 - vertex.y) * 0.5;
	output.color = color;

	return output;
}
)";

constexpr static const char p_shader_source[] = R"(
Texture2D tex;
SamplerState samp;

struct PixelIn
{
	float4 position : SV_POSITION;
	float2 texcoords : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(PixelIn input) : SV_TARGET
{
	return tex.Sample(samp, input.texcoords) * input.color;
}
)";

namespace Infinity
{
	Renderer2D::Renderer2D():
		m_v_buff(nullptr),
		m_i_buff(nullptr),
		m_model(nullptr),
		m_shader(nullptr),

		m_model_location(),
		m_projection_view_location(),
		
		m_color_location(),

		m_error(false),

		m_def_texture(nullptr),
		m_camera(nullptr),

		m_batches()
	{
		m_v_buff = VertexBuffer::CreateVertexBuffer({
			{ "vertex", DataType::FLOAT2 }
		});

		if (!m_v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D quad model");
			m_error = true;
			return;
		}

		float vertices[] =
		{
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f,  1.0f
		};

		if (!m_v_buff->SetData(vertices, sizeof(vertices)))
		{
			INFINITY_CORE_ERROR("Error setting Renderer2D quad model data");
			m_error = true;
			return;
		}

		m_i_buff = IndexBuffer::CreateIndexBuffer();

		if (!m_i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D quad model");
			m_error = true;
			return;
		}

		unsigned int indices[] =
		{
			0, 3, 1,
			0, 2, 3
		};

		if (!m_i_buff->SetData(indices, sizeof(indices), 6))
		{
			INFINITY_CORE_ERROR("Errir setting Renderer2D quad model data");
			m_error = true;
			return;
		}

		m_model = Model::CreateModel(1);

		m_model->SetVertexBuffer(0, m_v_buff);
		m_model->SetIndexBuffer(m_i_buff);

		m_shader = Shader::CreateShader({
			{ "vertex", DataType::FLOAT2 }
		});

		if (!m_shader->Init(v_shader_source, sizeof(v_shader_source), p_shader_source, sizeof(p_shader_source)))
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D shader");
			m_error = true;
			return;
		}

		if (!m_shader->DeclareConstants({
				{ "model",           DataType::MAT4 },
				{ "projection_view", DataType::MAT4 },
				{ "color",           DataType::FLOAT4 }
			}))
		{
			INFINITY_CORE_ERROR("Error declaring Renderer2D shader constants");
			m_error = true;
			return;
		}

		m_model_location           = m_shader->GetConstantLocation("model");
		m_projection_view_location = m_shader->GetConstantLocation("projection_view");
		m_color_location           = m_shader->GetConstantLocation("color");

		m_def_texture = Texture2D::CreateTexture();

		constexpr unsigned char pixels[4] = { 255, 255, 255, 255 };

		if (!m_def_texture->Init(1, 1, pixels))
		{
			INFINITY_CORE_ERROR("Error initializing default Renderer2D texture");
			m_error = true;
			return;
		}
	}

	Renderer2D::~Renderer2D()
	{
		if (m_def_texture)
		{
			m_def_texture->Destroy();
			delete m_def_texture;
		}

		if (m_shader)
		{
			m_shader->Destroy();
			delete m_shader;
		}

		if (m_model)
		{
			delete m_model;
		}

		if (m_v_buff)
		{
			m_v_buff->Destroy();
			delete m_v_buff;
		}

		if (m_i_buff)
		{
			m_i_buff->Destroy();
			delete m_i_buff;
		}
	}

	void Renderer2D::RenderSingle(const QuadParams &quad)
	{
		if (!m_error)
		{
			m_shader->Bind();

			if (!quad.texture) m_def_texture->Bind(0);
			else quad.texture->Bind(0);

			if (!m_shader->MapConstants())
			{
				INFINITY_CORE_ERROR("Error setting Renderer2D shader constants");
				m_error = true;
				return;
			}

			Mat4f model;

			model = MakeRollPitchYawRotation(0.0f, 0.0f, quad.rotation) * MakeScale(quad.size.x * 0.5f, quad.size.y * 0.5f, 1.0f);

			if (quad.centered)
			{
				model = Translate(model, quad.position.x, quad.position.y, 0.0f);
			}
			else
			{
				Vec2f center = quad.position + quad.size * 0.5f;
				model = Translate(model, center.x, center.y, 0.0f);
			}

			m_shader->SetConstantMat4(m_model_location, &model[0][0], true);

			Mat4f pv;
			if (m_camera) pv = m_camera->GetProjectionViewMatrix();

			m_shader->SetConstantMat4(m_projection_view_location, &pv[0][0], true);

			m_shader->SetConstant4f(m_color_location, quad.color.r, quad.color.g, quad.color.b, quad.color.a);

			m_shader->UnmapConstants();

			m_model->Bind();
			m_model->Render();
		}
	}

	void Renderer2D::BatchAdd(const QuadParams &quad)
	{
		if (m_batches.ContainsKey(quad.texture))
		{
			m_batches[quad.texture].Add(quad);
		}
		else
		{
			ArrayList<QuadParams> batch = { quad };
			m_batches[quad.texture] = std::move(batch);
		}
	}

	void Renderer2D::BatchClear()
	{
		m_batches.Clear();
	}

	void Renderer2D::BatchRender()
	{
		if (m_error) return;

		m_shader->Bind();
		m_model->Bind();

		if (!m_shader->MapConstants())
		{
			INFINITY_CORE_ERROR("Error mapping Renderer2D constants");
			m_error = true;
			return;
		}

		Mat4f pv;
		if (m_camera) pv = m_camera->GetProjectionViewMatrix();
		m_shader->SetConstantMat4(m_projection_view_location, &pv[0][0], true);

		m_shader->UnmapConstants();

		for (auto &batch : m_batches)
		{
			if (batch.key) batch.key->Bind(0);
			else m_def_texture->Bind(0);

			for (auto &quad : batch.value)
			{
				if (!m_shader->MapConstants())
				{
					INFINITY_CORE_ERROR("Error mapping Renderer2D constants");
					m_error = true;
					return;
				}

				Mat4f model = MakeRollPitchYawRotation(0.0f, 0.0f, quad.rotation) * MakeScale(quad.size.x * 0.5f, quad.size.y * 0.5f, 1.0f);

				if (quad.centered)
				{
					model = Translate(model, quad.position.x, quad.position.y, 0.0f);
				}
				else
				{
					Vec2f center = quad.position + quad.size * 0.5f;
					model = Translate(model, center.x, center.y, 0.0f);
				}

				m_shader->SetConstantMat4(m_model_location, &model[0][0], true);
				m_shader->SetConstant4f(m_color_location, quad.color.r, quad.color.g, quad.color.b, quad.color.a);

				m_shader->UnmapConstants();

				m_model->Render();
			}
		}
	}

	void Renderer2D::SetCamera(const Camera *camera) { m_camera = camera; }
}