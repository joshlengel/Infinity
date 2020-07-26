#include"InfinityPCH.h"

#include"renderer2d/Aleph/Component.h"

#include"Log.h"

namespace Infinity::Aleph
{
	Component::Component():
		m_left(),
		m_right(),
		m_bottom(),
		m_top(),
		m_red(0.0f),
		m_green(0.0f),
		m_blue(0.0f),
		m_alpha(1.0f)
	{}

	Component::~Component()
	{}

	void Component::SetWindowSize(unsigned int width, unsigned int height, float aspect_ratio)
	{
		if (m_left) m_left->SetWindowSize(width, height, aspect_ratio);
		if (m_right) m_right->SetWindowSize(width, height, aspect_ratio);
		if (m_bottom) m_bottom->SetWindowSize(width, height, aspect_ratio);
		if (m_top) m_top->SetWindowSize(width, height, aspect_ratio);
	}

	void Component::Render(Resource<Renderer2D> &renderer)
	{
		float left = -1.0f;
		float right = 1.0f;
		float bottom = -1.0f;
		float top = 1.0f;

		if (m_left) left = m_left->m_value;
		if (m_right) right = m_right->m_value;
		if (m_bottom) bottom = m_bottom->m_value;
		if (m_top) top = m_top->m_value;

		Infinity::Renderer2D::QuadParams quad = {};
		quad.position = { (left + right) * 0.5f, (bottom + top) * 0.5f };
		quad.size = { right - left, top - bottom };
		quad.color = { m_red, m_green, m_blue, m_alpha };

		renderer->DrawQuad(quad);
	}

	void Component::Update()
	{
		if (m_left) m_left->Update();
		if (m_right) m_right->Update();
		if (m_bottom) m_bottom->Update();
		if (m_top) m_top->Update();
	}

	void Component::SetLeftConstraint(const Resource<HorizontalConstraint> &left)
	{
		m_left = left;
		ResourceCast<Constraint>(m_left)->m_orientation = Constraint::Orientation::HORIZONTAL;
	}

	void Component::SetRightConstraint(const Resource<HorizontalConstraint> &right)
	{
		m_right = right;
		ResourceCast<Constraint>(m_right)->m_orientation = Constraint::Orientation::HORIZONTAL;
	}

	void Component::SetBottomConstraint(const Resource<VerticalConstraint> &bottom)
	{
		m_bottom = bottom;
		ResourceCast<Constraint>(m_bottom)->m_orientation = Constraint::Orientation::VERTICAL;
	}

	void Component::SetTopConstraint(const Resource<VerticalConstraint> &top)
	{
		m_top = top;
		ResourceCast<Constraint>(m_top)->m_orientation = Constraint::Orientation::VERTICAL;
	}

	void Component::SetColor(float red, float green, float blue, float alpha)
	{
		m_red = red;
		m_green = green;
		m_blue = blue;
		m_alpha = alpha;
	}

	Resource<Component> Component::CreateComponent()
	{
		return MakeResource<Component>();
	}
}