#pragma once

#include"Core.h"

#include"utils/data/Map.h"

namespace Infinity
{
	class VertexBuffer;
	class IndexBuffer;
	class Model;
	class VertexLayout;

	class INFINITY_API ModelLoader
	{
	private:
		struct ModelAsset
		{
			VertexBuffer *v_buff;
			IndexBuffer *i_buff;
			Model *model;
		};

		Map<const char*, ModelAsset> m_models;

	public:
		ModelLoader();
		~ModelLoader();

		void Destroy();

		Model *Load(const char *name, const char *filename, const VertexLayout &layout);
		Model *Load(const char *name, const char *filename, VertexLayout &&layout);

		Model *Get(const char *name);

	private:
		void Load(const char *filename, Model *model, VertexBuffer *vertex_buffer, IndexBuffer *index_buffer);
	};
}