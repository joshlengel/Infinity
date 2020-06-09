#include"VertexLayout.h"

namespace Infinity
{
	VertexLayout::Element::Element(const std::string &name, DataType type):
		offset(),
		count(),
		input_slot(),
		name(name),
		type(type)
	{}

	VertexLayout::Element::Element(std::string &&name, DataType type):
		offset(),
		count(),
		input_slot(),
		name(std::move(name)),
		type(type)
	{}

	VertexLayout::Element::Element(const std::string &name, DataType type, unsigned int input_slot):
		offset(),
		count(),
		input_slot(input_slot),
		name(name),
		type(type)
	{}

	VertexLayout::Element::Element(std::string &&name, DataType type, unsigned int input_slot):
		offset(),
		count(),
		input_slot(input_slot),
		name(std::move(name)),
		type(type)
	{}

	VertexLayout::VertexLayout(std::initializer_list<Element> list):
		m_num_elements(list.size()),
		m_elements((Element*)malloc(m_num_elements * sizeof(Element))),
		m_stride()
	{
		const Element *itr = list.begin();

		unsigned int offset = 0;
		for (unsigned int i = 0; i < m_num_elements; ++i)
		{
			Element &e = m_elements[i];
			memcpy(&e.name, &itr->name, sizeof(std::string)); // must copy string this way
			e.type = itr->type;
			e.input_slot = itr->input_slot;
			e.offset = offset;
			e.count = GetCount(itr->type);

			offset += GetBytes(itr->type);

			++itr;
		}

		m_stride = offset;
	}

	VertexLayout::VertexLayout(const VertexLayout &layout):
		m_num_elements(layout.m_num_elements),
		m_elements((Element*)malloc(layout.m_num_elements * sizeof(Element))),
		m_stride(layout.m_stride)
	{
		memcpy(m_elements, layout.m_elements, m_num_elements * sizeof(Element));
	}

	VertexLayout::VertexLayout(VertexLayout &&layout) noexcept:
		m_num_elements(layout.m_num_elements),
		m_elements(layout.m_elements),
		m_stride(layout.m_stride)
	{
		layout.m_num_elements = 0;
		layout.m_elements = nullptr;
		layout.m_stride = 0;
	}

	VertexLayout::~VertexLayout()
	{
		if (m_elements) free(m_elements);
	}

	VertexLayout &VertexLayout::operator=(const VertexLayout &layout)
	{
		unsigned int size = layout.m_num_elements * sizeof(Element);

		m_num_elements = layout.m_num_elements;
		m_elements = (Element*)malloc(size);
		m_stride = layout.m_stride;
		
		memcpy(m_elements, layout.m_elements, size);

		return *this;
	}

	VertexLayout &VertexLayout::operator=(VertexLayout &&layout) noexcept
	{
		m_num_elements = layout.m_num_elements;
		m_elements = layout.m_elements;
		m_stride = layout.m_stride;

		layout.m_num_elements = 0;
		layout.m_elements = nullptr;
		layout.m_stride = 0;

		return *this;
	}

	const VertexLayout::Element *VertexLayout::begin() const { return m_elements; }
	const VertexLayout::Element *VertexLayout::end() const { return m_elements + m_num_elements; }

	unsigned int VertexLayout::GetNumElements() const { return m_num_elements; }

	unsigned int VertexLayout::GetStride() const { return m_stride; }
}