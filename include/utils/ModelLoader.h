#pragma once

#include"Core.h"

#include"Model.h"
#include"VertexLayout.h"

#include"utils/data/Map.h"
#include"utils/data/String.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API ModelLoader
	{
	private:
		struct ModelAsset
		{
			Resource<VertexBuffer> v_buff;
			Resource<IndexBuffer> i_buff;
			Resource<Model> model;
		};

		Map<String, ModelAsset> m_models;

	public:
		ModelLoader();
		~ModelLoader();

		Resource<Model> Load(const String &name, const String &filename, const VertexLayout &layout);
		Resource<Model> Load(String &&name, const String &filename, const VertexLayout &layout);
		Resource<Model> Load(const String &name, const String &filename, VertexLayout &&layout);
		Resource<Model> Load(String &&name, const String &filename, VertexLayout &&layout);

		Resource<Model> Get(const String &name);

		void Remove(const String &name);

	private:
		void Load(const String &filename, Resource<Model> model, Resource<VertexBuffer> vertex_buffer, Resource<IndexBuffer> index_buffer);
	};
}