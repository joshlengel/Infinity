#pragma once

#include"Core.h"

#include"utils\Camera.h"
#include"utils\math\Math.h"

#include<vector>
#include<map>

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
			Vec2f position;
			Vec2f size;
			float rotation = 0.0f;
			bool centered = true;
			Vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

			const Texture2D *texture;
		};

	private:
		VertexBuffer *m_v_buff;
		IndexBuffer *m_i_buff;
		Model *m_model;
		Shader *m_shader;

		int m_projection_view_location;
		int m_model_location;

		int m_color_location;

		bool m_error;

		Texture2D *m_def_texture;
		const Camera *m_camera;

		std::map<const Texture2D*, std::vector<QuadParams>> m_batches;

	public:
		Renderer2D();
		~Renderer2D();

		void RenderSingle(const QuadParams &quad);

		void BatchAdd(const QuadParams &quad);
		void BatchClear();
		void BatchRender();

		void SetCamera(const Camera *camera);
	};
}