#include"InfinityPCH.h"

#include"renderer2d/Aleph/Constraint.h"

namespace Infinity::Aleph
{
	Constraint::Constraint(Orientation orientation):
		m_orientation(orientation),
		m_value(),
		m_window_width(),
		m_window_height(),
		m_window_aspect_ratio()
	{}

	Constraint::~Constraint()
	{}

	void Constraint::SetWindowSize(unsigned int width, unsigned int height, float aspect_ratio)
	{
		m_window_width = width;
		m_window_height = height;
		m_window_aspect_ratio = aspect_ratio;
	}

	HorizontalConstraint::HorizontalConstraint():
		Constraint(Orientation::HORIZONTAL)
	{}

	HorizontalConstraint::~HorizontalConstraint() {}

	VerticalConstraint::VerticalConstraint():
		Constraint(Orientation::VERTICAL)
	{}

	VerticalConstraint::~VerticalConstraint() {}

	PixelConstraint::PixelConstraint(unsigned int px):
		Constraint(Orientation::HORIZONTAL), // Default. This gets changed at runtime
		m_px(px)
	{}

	PixelConstraint::~PixelConstraint()
	{}

	void PixelConstraint::Update()
	{
		// It doesn't matter wether HorizontalConstraint's or VerticalConstraint's m_orientation is used
		if (m_orientation == Orientation::HORIZONTAL)
		{
			HorizontalConstraint::m_value = float(m_px) / float(HorizontalConstraint::m_window_width) * 2.0f - 1.0f;
		}
		else
		{
			VerticalConstraint::m_value = 1.0f - float(m_px) / float(VerticalConstraint::m_window_height) * 2.0f;
		}
	}

	Resource<PixelConstraint> PixelConstraint::CreatePixelConstraint(unsigned int px)
	{
		return MakeResource<PixelConstraint>(px);
	}
}