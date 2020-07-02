#include"InfinityPCH.h"

#include"VertexLayout.h"

namespace Infinity
{
	VertexLayout::Element::Element():
		offset(),
		count(),
		input_slot(),
		name(),
		type()
	{}

	VertexLayout::Element::Element(const String &name, DataType type):
		offset(),
		count(),
		input_slot(),
		name(name),
		type(type)
	{}

	VertexLayout::Element::Element(String &&name, DataType type):
		offset(),
		count(),
		input_slot(),
		name(std::forward<String>(name)),
		type(type)
	{}

	VertexLayout::Element::Element(const String &name, DataType type, unsigned int input_slot):
		offset(),
		count(),
		input_slot(input_slot),
		name(name),
		type(type)
	{}

	VertexLayout::Element::Element(String &&name, DataType type, unsigned int input_slot):
		offset(),
		count(),
		input_slot(input_slot),
		name(std::forward<String>(name)),
		type(type)
	{}

	VertexLayout::VertexLayout(std::initializer_list<Element> list):
		m_elements(list),
		m_stride()
	{
		const Element *itr = list.begin();

		unsigned int offset = 0;
		for (unsigned int i = 0; i < list.size(); ++i)
		{
			Element &e = m_elements[i];
			e.offset = offset;
			e.count = GetCount(itr->type);

			offset += GetBytes(itr->type);

			++itr;
		}

		m_stride = offset;
	}

	VertexLayout::VertexLayout(const VertexLayout &layout):
		m_elements(layout.m_elements),
		m_stride(layout.m_stride)
	{}

	VertexLayout::VertexLayout(VertexLayout &&layout) noexcept:
		m_elements(std::forward<ArrayList<Element>>(layout.m_elements)),
		m_stride(layout.m_stride)
	{
		layout.m_elements.Clear();
		layout.m_stride = 0;
	}

	VertexLayout &VertexLayout::operator=(const VertexLayout &layout)
	{
		m_elements = layout.m_elements;
		m_stride = layout.m_stride;

		return *this;
	}

	VertexLayout &VertexLayout::operator=(VertexLayout &&layout) noexcept
	{
		m_elements = std::forward<ArrayList<Element>>(layout.m_elements);
		m_stride = layout.m_stride;

		return *this;
	}

	const VertexLayout::Element *VertexLayout::begin() const { return m_elements.begin(); }
	const VertexLayout::Element *VertexLayout::end() const { return m_elements.end(); }

	unsigned int VertexLayout::GetNumElements() const { return m_elements.GetSize(); }

	unsigned int VertexLayout::GetStride() const { return m_stride; }
}