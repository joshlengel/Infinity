#pragma once

#include"Core.h"

#include"Model.h"
#include"Shader.h"
#include"Texture.h"

#include"Context.h"

#include"utils/Camera.h"
#include"utils/math/Math.h"

#include"utils/data/ArrayList.h"
#include"utils/data/Map.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API Renderer2D
	{
	public:
		struct QuadParams
		{
			Vec2f position = { 0.0f, 0.0f };
			Vec2f size = { 1.0f, 1.0f };
			Vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

			float rotation = 0.0f;

			bool batched = false;

			Resource<Texture2D> texture = nullptr;
		};

	private:
		template <typename T, typename ...Args>
		friend Resource<T> MakeResource<T>(Args&&... args);

		struct Vertex
		{
			Vec2f position;
			Vec4f color;
			Vec2f tex_coords;
		};

		struct RenderModel
		{
			Resource<VertexBuffer> v_buff;
			Resource<IndexBuffer> i_buff;
			Resource<Model> model;
			Resource<Shader> shader;
		};

		RenderModel m_batched, m_unbatched;

		int m_batched_projection_view_location;
		int m_unbatched_projection_view_location;
		int m_unbatched_model_location;
		int m_unbatched_color_location;

		bool m_error;

		Resource<Texture2D> m_def_texture;

		const static unsigned int MAX_QUADS = 300;
		const static unsigned int MAX_VERTICES = MAX_QUADS * 4;
		const static unsigned int MAX_INDICES = MAX_QUADS * 6;

		struct Batch
		{
			Vertex vertices[MAX_VERTICES] = {};
			Vertex *v_ptr = vertices;

			unsigned int index_count = 0;
		};

		Map<Resource<Texture2D>, Batch*> m_batches;

		Resource<Context> m_context;
		Resource<ContextSettings> m_restore_context_settings, m_context_settings;

		Renderer2D();
	
	public:
		~Renderer2D();

		bool Init();

		void StartScene(const Camera *camera);
		void DrawQuad(const QuadParams &quad);
		void EndScene();

		static Resource<Renderer2D> CreateRenderer2D();

	private:
		void Flush(Resource<Texture2D> texture, Batch *batch);
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Renderer2D>;
}