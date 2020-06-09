#pragma once

#include"Core.h"

#include"Type.h"

#include<string>

namespace Infinity
{
	class INFINITY_API VertexLayout
	{
	public:
		struct INFINITY_API Element
		{
			unsigned int offset;
			unsigned int count;

			unsigned int input_slot;

			std::string name;
			DataType type;

			Element(const std::string &name, DataType type);
			Element(std::string &&name, DataType type);

			Element(const std::string &name, DataType type, unsigned int input_slot);
			Element(std::string &&name, DataType type, unsigned int input_slot);
		};

	private:
		unsigned int m_num_elements;
		Element *m_elements;

		unsigned int m_stride;

	public:
		VertexLayout(std::initializer_list<Element> list);
		VertexLayout(const VertexLayout &layout);
		VertexLayout(VertexLayout &&layout) noexcept;
		~VertexLayout();

		VertexLayout &operator=(const VertexLayout &layout);
		VertexLayout &operator=(VertexLayout &&layout) noexcept;

		const Element *begin() const;
		const Element *end() const;

		unsigned int GetNumElements() const;

		unsigned int GetStride() const;
	};
}