#include"InfinityPCH.h"

#include"renderer2d/Renderer2D.h"

#include"Model.h"
#include"Shader.h"
#include"Type.h"

#include"Texture.h"

#include"Renderer2DShaderSource.cpp" // contains the source for shaders

namespace Infinity
{
	Renderer2D::Renderer2D():
		m_batched(),
		m_unbatched(),
		
		m_batched_projection_view_location(),
		m_unbatched_projection_view_location(),
		m_unbatched_model_location(),
		m_unbatched_color_location(),

		m_error(false),

		m_def_texture(nullptr),

		m_batches()
	{}

	Renderer2D::~Renderer2D()
	{
		for (auto &entry : m_batches)
		{
			delete entry.value;
		}
	}

	bool Renderer2D::Init()
	{
		m_batched.v_buff = VertexBuffer::CreateVertexBuffer({
			{ String("position"),   DataType::FLOAT2 },
			{ String("color"),      DataType::FLOAT4 },
			{ String("tex_coords"), DataType::FLOAT2 }
		});

		m_unbatched.v_buff = VertexBuffer::CreateVertexBuffer({
			{ String("vertex"),     DataType::FLOAT2 },
			{ String("tex_coords"), DataType::FLOAT2 }
		});

		if (!m_batched.v_buff->Init(true) || !m_unbatched.v_buff->Init(false))
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D quad model");
			m_error = true;
			return false;
		}

		float unbatched_vertices[] =
		{
			-1.0f, -1.0f,   0.0f, 1.0f,
			 1.0f, -1.0f,   1.0f, 1.0f,
			-1.0f,  1.0f,   0.0f, 0.0f,
			 1.0f,  1.0f,   1.0f, 0.0f
		};

		if (!m_batched.v_buff->SetData(nullptr, MAX_VERTICES * sizeof(Vertex)) || !m_unbatched.v_buff->SetData(unbatched_vertices, sizeof(unbatched_vertices)))
		{
			INFINITY_CORE_ERROR("Error setting Renderer2D quad model data");
			m_error = true;
			return false;
		}

		m_batched.i_buff = IndexBuffer::CreateIndexBuffer();
		m_unbatched.i_buff = IndexBuffer::CreateIndexBuffer();

		if (!m_batched.i_buff->Init(true) || !m_unbatched.i_buff->Init(false))
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D indices");
			m_error = true;
			return false;
		}

		unsigned int *batched_indices = new unsigned int[MAX_INDICES];

		unsigned int index = 0;
		for (unsigned int i = 0; i < MAX_INDICES; i += 6)
		{
			batched_indices[i + 0] = index + 0;
			batched_indices[i + 1] = index + 3;
			batched_indices[i + 2] = index + 1;
			batched_indices[i + 3] = index + 0;
			batched_indices[i + 4] = index + 2;
			batched_indices[i + 5] = index + 3;

			index += 4;
		}

		unsigned int unbatched_indices[] =
		{
			0, 3, 1,
			0, 2, 3
		};

		if (!m_batched.i_buff->SetData(batched_indices, MAX_INDICES * sizeof(unsigned int), MAX_INDICES)
			|| !m_unbatched.i_buff->SetData(unbatched_indices, sizeof(unbatched_indices), 6))
		{
			INFINITY_CORE_ERROR("Errir setting Renderer2D indices");
			m_error = true;
			return false;
		}

		delete[] batched_indices;

		m_batched.model = Model::CreateModel(1);
		m_unbatched.model = Model::CreateModel(1);
		
		m_batched.model->SetVertexBuffer(0, m_batched.v_buff);
		m_batched.model->SetIndexBuffer(m_batched.i_buff);

		m_unbatched.model->SetVertexBuffer(0, m_unbatched.v_buff);
		m_unbatched.model->SetIndexBuffer(m_unbatched.i_buff);

		m_batched.shader = Shader::CreateShader({
			{ "position",   DataType::FLOAT2, 0 },
			{ "color",      DataType::FLOAT4, 0 },
			{ "tex_coords", DataType::FLOAT2, 0 }
		});

		m_unbatched.shader = Shader::CreateShader({
			{ "vertex",     DataType::FLOAT2, 0 },
			{ "tex_coords", DataType::FLOAT2, 0 }
		});

		if (!m_batched.shader->Init(renderer2D_batched_v_source, renderer2D_p_source)
			|| !m_unbatched.shader->Init(renderer2D_unbatched_v_source, renderer2D_p_source))
		{
			INFINITY_CORE_ERROR("Error initializing Renderer2D shader");
			m_error = true;
			return false;
		}

		if (!m_batched.shader->DeclareConstants({
				{ "projection_view", DataType::MAT4 }
			}) || !m_unbatched.shader->DeclareConstants({
				{ "model",           DataType::MAT4   },
				{ "projection_view", DataType::MAT4   },
				{ "color",           DataType::FLOAT4 }
			}))
		{
			INFINITY_CORE_ERROR("Error declaring Renderer2D shader constants");
			m_error = true;
			return false;
		}

		m_batched_projection_view_location = m_batched.shader->GetConstantLocation("projection_view");
		m_unbatched_projection_view_location = m_unbatched.shader->GetConstantLocation("projection_view");
		m_unbatched_model_location = m_unbatched.shader->GetConstantLocation("model");
		m_unbatched_color_location = m_unbatched.shader->GetConstantLocation("color");

		m_def_texture = Texture2D::CreateTexture();

		constexpr unsigned char pixels[4] = { 255, 255, 255, 255 };

		if (!m_def_texture->Init(1, 1, pixels))
		{
			INFINITY_CORE_ERROR("Error initializing default Renderer2D texture");
			m_error = true;
			return false;
		}

		return true;
	}

	void Renderer2D::StartScene(const Camera *camera)
	{
		const Mat4f &pv = camera? camera->GetProjectionViewMatrix() : Mat4f();

		// batched
		m_batched.shader->Bind();

		if (!m_batched.shader->MapConstants())
		{
			INFINITY_CORE_ERROR("Error mapping Renderer2D constants");
			return;
		}

		m_batched.shader->SetConstantMat4(m_batched_projection_view_location, &pv[0][0], true);

		m_batched.shader->UnmapConstants();

		// unbatched
		m_unbatched.shader->Bind();

		if (!m_unbatched.shader->MapConstants())
		{
			INFINITY_CORE_ERROR("Error mapping Renderer2D constants");
			return;
		}

		m_unbatched.shader->SetConstantMat4(m_unbatched_projection_view_location, &pv[0][0], true);

		m_unbatched.shader->UnmapConstants();
	}

	void Renderer2D::DrawQuad(const QuadParams &quad)
	{
		Resource<Texture2D> texture = quad.texture;

		if (!texture) texture = m_def_texture;

		if (!quad.batched)
		{
			m_unbatched.shader->Bind();

			if (!m_unbatched.shader->MapConstants())
			{
				INFINITY_CORE_ERROR("Error mapping Renderer2D constants");
				return;
			}

			Mat4f model = MakeScale(quad.size.x * 0.5f, quad.size.y * 0.5f, 0.0f);

			float sinr = sin(quad.rotation);
			float cosr = cos(quad.rotation);
			model = Mat4f
			{
				 cosr, sinr, 0.0f, 0.0f,
				-sinr, cosr, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f
			} * model;

			model = Translate(model, quad.position.x, quad.position.y, 0.0f);

			m_unbatched.shader->SetConstantMat4(m_unbatched_model_location, &model[0][0], true);
			m_unbatched.shader->SetConstant4f(m_unbatched_color_location, quad.color.r, quad.color.g, quad.color.b, quad.color.a);

			m_unbatched.shader->UnmapConstants();

			texture->Bind(0);

			m_unbatched.model->Bind();
			m_unbatched.model->Render();

			return;
		}

		Batch *&batch = m_batches[texture];

		if (!batch)
		{
			batch = new Batch;
		}

		float sinr = sin(quad.rotation);
		float cosr = cos(quad.rotation);

		Mat2f model =
		{
			 cosr, sinr,
			-sinr, cosr
		};

		Mat2f scale =
		{
			quad.size.x * 0.5f,               0.0f,
			              0.0f, quad.size.y * 0.5f
		};

		model = model * scale;

		Vec2f p[4] =
		{
			Vec2f{ -1.0f, -1.0f } * model + quad.position,
			Vec2f{  1.0f, -1.0f } * model + quad.position,
			Vec2f{ -1.0f,  1.0f } * model + quad.position,
			Vec2f{  1.0f,  1.0f } * model + quad.position
		};

		Vec2f tc[4] =
		{
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f }
		};

		for (unsigned int i = 0; i < 4; ++i)
		{
			batch->v_ptr->position.x = p[i].x;
			batch->v_ptr->position.y = p[i].y;

			batch->v_ptr->color.r = quad.color.r;
			batch->v_ptr->color.g = quad.color.g;
			batch->v_ptr->color.b = quad.color.b;
			batch->v_ptr->color.a = quad.color.a;

			batch->v_ptr->tex_coords.x = tc[i].x;
			batch->v_ptr->tex_coords.y = tc[i].y;

			++batch->v_ptr;
		}

		batch->index_count += 6;

		if (batch->index_count == MAX_INDICES)
		{
			Flush(texture, batch);
		}
	}

	void Renderer2D::Flush(Resource<Texture2D> texture, Batch *batch)
	{
		texture->Bind(0);

		if (!m_batched.v_buff->SetData(batch->vertices, sizeof(batch->vertices)))
		{
			INFINITY_CORE_ERROR("Error setting Renderer2D vertex data");
			return;
		}

		m_batched.shader->Bind();
		m_batched.model->Bind();
		m_batched.model->Render(batch->index_count);

		batch->index_count = 0;

		batch->v_ptr = batch->vertices;
	}

	void Renderer2D::EndScene()
	{
		for (auto &entry : m_batches)
		{
			if (entry.value->index_count)
			{
				Flush(entry.key, entry.value);
			}
		}
	}
}