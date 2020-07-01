#include"InfinityPCH.h"

#include"utils/ModelLoader.h"

#include"Model.h"
#include"VertexLayout.h"

#include"utils/profiling/Timer.h"

const char *GetExtension(const char *filename)
{
	const char *itr = filename;

	unsigned int cache_index = 0;
	unsigned int index = 0;

	while (*itr)
	{
		if (*itr == '.') cache_index = index;

		++itr;
		++index;
	}

	if (!cache_index) return itr; // empty string
	else return filename + cache_index + 1;
}

bool StartsWith(const char *str, const char *prefix)
{
	while (true)
	{
		if (!*prefix) return true;

		if (*prefix != *str) return false;

		++str;
		++prefix;
	}
}

struct SplitResult
{
	const char *token;
	unsigned int length;
};

SplitResult Split(const char *&str, char delimiter)
{
	const char *start = str;

	while (*start == delimiter) ++start;

	const char *end = start;
	unsigned int length = 0;

	while (*end && *end != delimiter) { ++end; ++length; }

	str = end;

	return { start, length };
}

namespace Infinity
{
	ModelLoader::ModelLoader():
		m_models()
	{}

	ModelLoader::~ModelLoader()
	{
		m_models.Clear();
	}

	void LoadOBJ(std::ifstream &file, Resource<VertexBuffer> vertex_buffer, Resource<IndexBuffer> index_buffer)
	{
		INFINITY_PROFILE_SCOPE("LOAD OBJ");

		struct Vertex
		{
			Vec3f position;
			unsigned int tex_index, norm_index;
			bool set = false;
			unsigned int index;

			Vertex *similar = nullptr;

			Vertex(): Vertex({}, 0) {};
			Vertex(const Vec3f &position, unsigned int index): position(position), tex_index(0), norm_index(0), index(index) {}
			
			bool operator==(const Vertex &v) const { return position == v.position && tex_index == v.tex_index && norm_index == v.norm_index; }
		};

		ArrayList<Vertex*> v_comb;
		ArrayList<Vec2f> tex_coords;
		ArrayList<Vec3f> normals;

		bool use_tex_coords = false;
		bool use_normals = false;

		std::string line;

		while (std::getline(file, line))
		{
			const char *l = line.c_str();

			if (StartsWith(l, "v "))
			{
				const char *start = l + 2; // skip v and space characters

				auto[v1, v1_length] = Split(start, ' ');
				auto[v2, v2_length] = Split(start, ' ');
				auto[v3, v3_length] = Split(start, ' ');

				v_comb.Add(new Vertex({ strtof(v1, nullptr), strtof(v2, nullptr), strtof(v3, nullptr) }, v_comb.GetSize()));
			}
			else if (StartsWith(l, "vt "))
			{
				const char *start = l + 3; // skip vt and space characters

				auto[tc1, tc1_length] = Split(start, ' ');
				auto[tc2, tc2_length] = Split(start, ' ');

				tex_coords.Add({ strtof(tc1, nullptr), strtof(tc2, nullptr) });

				use_tex_coords = true;
			}
			else if (StartsWith(l, "vn "))
			{
				const char *start = l + 3; // skip vn and space characters

				auto[n1, n1_length] = Split(start, ' ');
				auto[n2, n2_length] = Split(start, ' ');
				auto[n3, n3_length] = Split(start, ' ');

				normals.Add({ strtof(n1, nullptr), strtof(n2, nullptr), strtof(n3, nullptr) });

				use_normals = true;
			}
			else if (StartsWith(l, "f ")) break;
		}

		ArrayList<unsigned int> indices;

		do
		{
			const char *l = line.c_str();

			if (StartsWith(l, "f "))
			{
				const char *start = l + 2;

				for (unsigned int i = 0; i < 3; ++i)
				{
					auto[group, group_length] = Split(start, ' ');

					auto[position, position_length] = Split(group, '/');
					auto[tex_coord, tex_coord_length] = Split(group, '/');
					auto[normal, normal_length] = Split(group, '/');

					unsigned int pos_i = (unsigned)atoi(position) - 1;
					unsigned int tc_i = use_tex_coords? (unsigned)atoi(tex_coord) - 1 : 0;
					unsigned int norm_i = use_normals? (unsigned)atoi(normal) - 1 : 0;
					
					Vertex *v = v_comb[pos_i];

					if (v->set)
					{
						if (v->tex_index == tc_i && v->norm_index == norm_i)
						{
							indices.Add(v->index);
						}
						else
						{
							// go down the chain until an unset similar vertex is found
							Vertex *next = v;

							while (next->similar)
							{
								if (next->similar->set && next->similar->tex_index == tc_i && next->similar->norm_index == norm_i) // check if vertex already exists
								{
									indices.Add(next->similar->index);
									goto no_duplicate;
								}

								next = next->similar;
							}

							// no duplicate exists, create new
							next->similar = new Vertex(next->position, v_comb.GetSize());
							next->similar->set = true;
							next->similar->tex_index = tc_i;
							next->similar->norm_index = norm_i;

							indices.Add(next->similar->index);

							v_comb.Add(next->similar);

						no_duplicate:;
						}
					}
					else
					{
						v->set = true;
						v->tex_index = tc_i;
						v->norm_index = norm_i;

						indices.Add(v->index);
					}
				}
			}
		}
		while (std::getline(file, line));

		const VertexLayout &layout = vertex_buffer->GetLayout();

		unsigned int arr_size = layout.GetStride() * v_comb.GetSize(); 
		float *arr = new float[arr_size / sizeof(float)]; 

		unsigned int arr_i = 0;
		for (Vertex *v : v_comb)
		{
			// position
			if (layout.GetNumElements() > 0 && layout.begin()->type == DataType::FLOAT3)
			{
				arr[arr_i++] = v->position.x;
				arr[arr_i++] = v->position.y;
				arr[arr_i++] = v->position.z;
			}

			// tex coords
			if (layout.GetNumElements() > 1 && (layout.begin() + 1)->type == DataType::FLOAT2)
			{
				Vec2f tex = tex_coords[v->tex_index];
				arr[arr_i++] = tex.x;
				arr[arr_i++] = tex.y;
			}

			// normal
			if (layout.GetNumElements() > 2 && (layout.begin() + 2)->type == DataType::FLOAT3 || layout.GetNumElements() > 1 && (layout.begin() + 1)->type == DataType::FLOAT3)
			{
				Vec3f norm = normals[v->norm_index];
				arr[arr_i++] = norm.x;
				arr[arr_i++] = norm.y;
				arr[arr_i++] = norm.z;
			}

			delete v;
		}

		if (!vertex_buffer->SetData(arr, arr_size))
		{
			INFINITY_CORE_ERROR("Error loading OBJ data into VertexBuffer");
		}

		delete[] arr;

		unsigned int arr_indices_size = indices.GetSize() * sizeof(unsigned int);
		unsigned int *arr_indices = new unsigned int[indices.GetSize()];
		memcpy(arr_indices, indices.begin(), arr_indices_size);

		if (!index_buffer->SetData(arr_indices, arr_indices_size, indices.GetSize()))
		{
			INFINITY_CORE_ERROR("Error loading OBJ data into VertexBuffer");
		}

		delete[] arr_indices;
	}

	void ModelLoader::Load(const String &filename, Resource<Model> model, Resource<VertexBuffer> vertex_buffer, Resource<IndexBuffer> index_buffer)
	{
		std::ifstream file(&filename[0]);

		if (!file)
		{
			INFINITY_CORE_ERROR("Error finding model file '{0}'", &filename[0]);
			return;
		}

		const char *extension = GetExtension(&filename[0]);

		if (strcmp(extension, "obj") == 0)
		{
			INFINITY_CORE_INFO("Loading '{0}' as OBJ file", &filename[0]);
			LoadOBJ(file, vertex_buffer, index_buffer);
		}
		else
		{
			INFINITY_CORE_ERROR("Unknown model file format '{0}'", extension);
		}

		model->SetVertexBuffer(0, vertex_buffer);
		model->SetIndexBuffer(index_buffer);
	}

	Resource<Model> ModelLoader::Load(const String &name, const String &filename, const VertexLayout &layout)
	{
		Resource<VertexBuffer> v_buff = VertexBuffer::CreateVertexBuffer(layout);
		Resource<IndexBuffer> i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Resource<Model> model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);

		auto entry = Find(m_models, name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff = v_buff;
			asset.i_buff = i_buff;
			asset.model = model;
		}
		else
		{
			m_models[name] = { v_buff, i_buff, model };
		}

		return model;
	}

	Resource<Model> ModelLoader::Load(String &&name, const String &filename, const VertexLayout &layout)
	{
		Resource<VertexBuffer> v_buff = VertexBuffer::CreateVertexBuffer(layout);
		Resource<IndexBuffer> i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Resource<Model> model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);
		
		auto entry = Find(m_models, name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff = v_buff;
			asset.i_buff = i_buff;
			asset.model = model;
		}
		else
		{
			m_models[std::forward<String>(name)] = { v_buff, i_buff, model };
		}

		return model;
	}

	Resource<Model> ModelLoader::Load(const String &name, const String &filename, VertexLayout &&layout)
	{
		Resource<VertexBuffer> v_buff = VertexBuffer::CreateVertexBuffer(std::forward<VertexLayout>(layout));
		Resource<IndexBuffer> i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Resource<Model> model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);

		auto entry = Find(m_models, name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff = v_buff;
			asset.i_buff = i_buff;
			asset.model = model;
		}
		else
		{
			m_models[name] = { v_buff, i_buff, model };
		}

		return model;
	}

	Resource<Model> ModelLoader::Load(String &&name, const String &filename, VertexLayout &&layout)
	{
		Resource<VertexBuffer> v_buff = VertexBuffer::CreateVertexBuffer(std::forward<VertexLayout>(layout));
		Resource<IndexBuffer> i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Resource<Model> model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);

		auto entry = Find(m_models, name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff = v_buff;
			asset.i_buff = i_buff;
			asset.model = model;
		}
		else
		{
			m_models[std::forward<String>(name)] = { v_buff, i_buff, model };
		}

		return model;
	}

	Resource<Model> ModelLoader::Get(const String &name)
	{
		auto itr = Find(m_models, name);

		return itr == m_models.end()? nullptr : itr->value.model;
	}

	void ModelLoader::Remove(const String &name)
	{
		Infinity::Remove(m_models, name);
	}
}