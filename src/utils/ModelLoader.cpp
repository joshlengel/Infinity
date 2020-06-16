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

const char *AdvanceWhitespace(const char *str, unsigned int &len)
{
	len = 0;

	while (*str && *str != ' ') { ++str; ++len; }
	while (*str && *str == ' ') ++str;

	return str;
}

const char *Locate(const char *str, char c, unsigned int &len)
{
	len = 0;

	while (*str && *str != c) { ++str; ++len; }

	return str;
}

namespace Infinity
{
	ModelLoader::ModelLoader():
		m_models()
	{}

	ModelLoader::~ModelLoader()
	{}

	void ModelLoader::Destroy()
	{
		for (auto &entry : m_models)
		{
			ModelAsset &asset = entry.value;

			if (asset.v_buff)
			{
				asset.v_buff->Destroy();
				delete asset.v_buff;
				asset.v_buff = nullptr;
			}

			if (asset.i_buff)
			{
				asset.i_buff->Destroy();
				delete asset.i_buff;
				asset.i_buff = nullptr;
			}

			if (asset.model)
			{
				delete asset.model;
				asset.model = nullptr;
			}
		}
	}

	void LoadOBJ(std::ifstream &file, VertexBuffer *vertex_buffer, IndexBuffer *index_buffer)
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

		{
			INFINITY_PROFILE_SCOPE("LOAD OBJ VERTICES");
		while (std::getline(file, line))
		{
			unsigned int len;

			const char *l = *line.c_str() == ' '? AdvanceWhitespace(line.c_str(), len) : line.c_str();

			if (StartsWith(l, "v "))
			{
				float v[3];

				const char *num = l;
				const char *num_end = AdvanceWhitespace(l, len);

				for (unsigned int i = 0; i < 3; ++i)
				{
					num = num_end;
					num_end = AdvanceWhitespace(num, len); // len has length of num

					if (len >= 20)
					{
						INFINITY_CORE_WARN("OBJ loading only supports numbers with less than {0} digits. Numbers will be truncated", 20);
					}
					else
					{
						char buff[20];
						memset(buff, 0, sizeof(buff));
						memcpy(buff, num, min(len * sizeof(char), sizeof(buff)));

						v[i] = strtof(num, nullptr);
					}
				}

				v_comb.Add(new Vertex(v, v_comb.GetSize()));
			}
			else if (StartsWith(l, "vt "))
			{
				float v[2];

				const char *num = l;
				const char *num_end = AdvanceWhitespace(l, len);

				for (unsigned int i = 0; i < 2; ++i)
				{
					num = num_end;
					num_end = AdvanceWhitespace(num, len); // len has length of num

					if (len >= 20)
					{
						INFINITY_CORE_WARN("OBJ loading only supports numbers with less than {0} digits. Numbers will be truncated", 20);
					}
					else
					{
						char buff[20];
						memset(buff, 0, sizeof(buff));
						memcpy(buff, num, min(len * sizeof(char), sizeof(buff)));

						v[i] = strtof(num, nullptr);
					}
				}

				tex_coords.Add(v);

				use_tex_coords = true;
			}
			else if (StartsWith(l, "vn "))
			{
				float v[3];

				const char *num = l;
				const char *num_end = AdvanceWhitespace(l, len);

				for (unsigned int i = 0; i < 3; ++i)
				{
					num = num_end;
					num_end = AdvanceWhitespace(num, len); // len has length of num

					if (len >= 20)
					{
						INFINITY_CORE_WARN("OBJ loading only supports numbers with less than {0} digits. Numbers will be truncated", 20);
					}
					else
					{
						char buff[20];
						memset(buff, 0, sizeof(buff));
						memcpy(buff, num, min(len * sizeof(char), sizeof(buff)));

						v[i] = strtof(num, nullptr);
					}
				}

				normals.Add(v);

				use_normals = true;
			}
			else if (StartsWith(l, "f ")) break;
		}
		}

		ArrayList<unsigned int> indices;

		{
			INFINITY_PROFILE_SCOPE("GENERATE OBJ INDICES");
		do
		{
			unsigned int len;

			const char *l = *line.c_str() == ' '? AdvanceWhitespace(line.c_str(), len) : line.c_str();

			if (StartsWith(l, "f "))
			{
				const char *group = l;
				const char *group_end = AdvanceWhitespace(l, len);

				for (unsigned int i = 0; i < 3; ++i)
				{
					group = group_end;
					group_end = AdvanceWhitespace(group, len);

					unsigned int pos_i, tex_i, norm_i;
					
					unsigned int temp;

					const char *num = group;
					const char *num_end = Locate(num, '/', temp);

					if (num_end > group_end) num_end = group + len;
						
					char buff[7];
					memset(buff, 0, sizeof(buff));
					memcpy(buff, num, min(num_end - num, sizeof(buff)));

					++num_end;

					pos_i = (unsigned)atoi(buff) - 1;
					Vertex *v = v_comb[pos_i];

					if (use_tex_coords)
					{
						num = num_end;
						num_end = Locate(num, '/', temp);

						if (num_end > group_end) num_end = group + len;
							
						memset(buff, 0, sizeof(buff));
						memcpy(buff, num, min(num_end - num, sizeof(buff)));

						++num_end;

						tex_i = (unsigned)atoi(buff) - 1;
					}

					if (use_normals)
					{
						num = num_end;
						num_end = Locate(num, '/', temp);

						if (num_end > group_end) num_end = group + len;
							
						memset(buff, 0, sizeof(buff));
						memcpy(buff, num, min(num_end - num, sizeof(buff)));

						++num_end;

						norm_i = (unsigned)atoi(buff) - 1;
					}

					if (v->set)
					{
						if (v->tex_index == tex_i && v->norm_index == norm_i)
						{
							indices.Add(v->index);
						}
						else
						{
							// go down the chain until an unset similar vertex is found
							Vertex *next = v;

							while (next->similar)
							{
								if (next->similar->set && next->similar->tex_index == tex_i && next->similar->norm_index == norm_i) // check if vertex already exists
								{
									indices.Add(next->similar->index);
									goto no_duplicate;
								}

								next = next->similar;
							}

							// no duplicate exists, create new
							next->similar = new Vertex(next->position, v_comb.GetSize());
							next->similar->set = true;
							next->similar->tex_index = tex_i;
							next->similar->norm_index = norm_i;

							indices.Add(next->similar->index);

							v_comb.Add(next->similar);

						no_duplicate:
							;
						}
					}
					else
					{
						v->set = true;
						v->tex_index = tex_i;
						v->norm_index = norm_i;

						indices.Add(v->index);
					}
				}
			}
		}
		while (std::getline(file, line));
		}

		{
		INFINITY_PROFILE_SCOPE("LOAD OBJ VERTEX DATA");
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
		}

		for (Vertex *v : v_comb)
		{
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
	}

	void ModelLoader::Load(const char *filename, Model *model, VertexBuffer *vertex_buffer, IndexBuffer *index_buffer)
	{
		std::ifstream file(filename);

		if (!file)
		{
			INFINITY_CORE_ERROR("Error finding model file '{0}'", filename);
			return;
		}

		const char *extension = GetExtension(filename);

		if (strcmp(extension, "obj") == 0)
		{
			INFINITY_CORE_INFO("Loading '{0}' as OBJ file", filename);
			LoadOBJ(file, vertex_buffer, index_buffer);
		}
		else
		{
			INFINITY_CORE_ERROR("Unknown model file format '{0}'", extension);
		}

		model->SetVertexBuffer(0, vertex_buffer);
		model->SetIndexBuffer(index_buffer);
	}

	Model *ModelLoader::Load(const char *name, const char *filename, const VertexLayout &layout)
	{
		VertexBuffer *v_buff = VertexBuffer::CreateVertexBuffer(layout);
		IndexBuffer *i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Model *model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);

		auto entry = m_models.Find(name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff->Destroy();
			asset.i_buff->Destroy();

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

	Model *ModelLoader::Load(const char *name, const char *filename, VertexLayout &&layout)
	{
		VertexBuffer *v_buff = VertexBuffer::CreateVertexBuffer(std::move(layout));
		IndexBuffer *i_buff = IndexBuffer::CreateIndexBuffer();

		if (!v_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing vertex buffer in model loader");
		}

		if (!i_buff->Init())
		{
			INFINITY_CORE_ERROR("Error initializing index buffer in model loader");
		}

		Model *model = Model::CreateModel(1);

		Load(filename, model, v_buff, i_buff);

		auto entry = m_models.Find(name);

		if (entry != m_models.end())
		{
			ModelAsset &asset = entry->value;
			asset.v_buff->Destroy();
			asset.i_buff->Destroy();

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

	Model *ModelLoader::Get(const char *name)
	{
		return m_models[name].model;
	}
}