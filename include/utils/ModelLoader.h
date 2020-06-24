#pragma once

#include"Core.h"

#include"utils/data/Map.h"
#include"utils/data/String.h"

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

		Map<StaticString, ModelAsset> m_models;

	public:
		ModelLoader();
		~ModelLoader();

		void Destroy();

		Model *Load(StaticString name, const char *filename, const VertexLayout &layout);
		Model *Load(StaticString name, const char *filename, VertexLayout &&layout);

		Model *Get(StaticString name);

		void Destroy(StaticString name);

	private:
		void Load(const char *filename, Model *model, VertexBuffer *vertex_buffer, IndexBuffer *index_buffer);
	};
}