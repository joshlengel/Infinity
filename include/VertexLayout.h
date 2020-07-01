#pragma once

#include"Core.h"

#include"Type.h"

#include"utils/data/ArrayList.h"
#include"utils/data/String.h"

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

			String name;
			DataType type;

			Element();

			Element(const String &name, DataType type);
			Element(String &&name, DataType type);

			Element(const String &name, DataType type, unsigned int input_slot);
			Element(String &&name, DataType type, unsigned int input_slot);
		};

	private:
		ArrayList<Element> m_elements;

		unsigned int m_stride;

	public:
		VertexLayout(std::initializer_list<Element> list);
		VertexLayout(const VertexLayout &layout);
		VertexLayout(VertexLayout &&layout) noexcept;
		
		VertexLayout &operator=(const VertexLayout &layout);
		VertexLayout &operator=(VertexLayout &&layout) noexcept;

		const Element *begin() const;
		const Element *end() const;

		unsigned int GetNumElements() const;

		unsigned int GetStride() const;
	};

	INFINITY_TEMPLATE template class INFINITY_API ArrayList<VertexLayout::Element>;
}