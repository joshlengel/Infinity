#pragma once

#include"Core.h"

#include"utils\Camera.h"
#include"utils\math\Math.h"

#include"utils/data/ArrayList.h"
#include"utils/data/Map.h"

namespace Infinity
{
	class VertexBuffer;
	class IndexBuffer;
	class Model;
	class Shader;
	class Texture2D;

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

			const Texture2D *texture = nullptr;
		};

	private:
		struct Vertex
		{
			Vec2f position;
			Vec4f color;
			Vec2f tex_coords;
		};

	private:

		struct RenderModel
		{
			VertexBuffer *v_buff;
			IndexBuffer *i_buff;
			Model *model;
			Shader *shader;

			~RenderModel();
			void Destroy();
		};

		RenderModel m_batched, m_unbatched;

		int m_batched_projection_view_location;
		int m_unbatched_projection_view_location;
		int m_unbatched_model_location;
		int m_unbatched_color_location;

		bool m_error;

		Texture2D *m_def_texture;

		const static unsigned int MAX_QUADS = 300;
		const static unsigned int MAX_VERTICES = MAX_QUADS * 4;
		const static unsigned int MAX_INDICES = MAX_QUADS * 6;

		struct Batch
		{
			Vertex vertices[MAX_VERTICES] = {};
			Vertex *v_ptr = vertices;

			unsigned int index_count = 0;
		};

		Map<const Texture2D*, Batch*> m_batches;

	public:
		Renderer2D();
		~Renderer2D();

		bool Init();
		void Destroy();

		void StartScene(const Camera *camera);
		void DrawQuad(const QuadParams &quad);
		void EndScene();

	private:
		void Flush(const Texture2D *texture, Batch *batch);
	};
}