#pragma once

#include"Core.h"

#include"renderer2d/Renderer2D.h"

#include"utils/data/ArrayList.h"
#include"utils/data/Resource.h"

#include"Constraint.h"

namespace Infinity
{
	namespace Aleph
	{
		class Component;
	}
	
	INFINITY_TEMPLATE template class INFINITY_API ResourceView<Aleph::Component>;
	INFINITY_TEMPLATE template class INFINITY_API ResourceFromThis<Aleph::Component>;
	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::Component>;
	INFINITY_TEMPLATE template class INFINITY_API ArrayList<Resource<Aleph::Component>>;

	namespace Aleph
	{
		class INFINITY_API Component : public ResourceFromThis<Component>
		{
			template <typename T, typename ...Args>
			friend Resource<T> Infinity::MakeResource(Args&&... args);

			friend class GuiCore;

		protected:
			Resource<HorizontalConstraint> m_left, m_right;
			Resource<VerticalConstraint> m_bottom, m_top;

			float m_red, m_green, m_blue, m_alpha;

			Component();

		public:
			virtual ~Component();

		protected:
			virtual void Render(Resource<Renderer2D> &renderer);
			virtual void Update();
		
			void SetWindowSize(unsigned int width, unsigned int height, float aspect_ratio);

		public:
			void SetLeftConstraint(const Resource<HorizontalConstraint> &left);
			void SetRightConstraint(const Resource<HorizontalConstraint> &right);
			void SetBottomConstraint(const Resource<VerticalConstraint> &bottom);
			void SetTopConstraint(const Resource<VerticalConstraint> &top);

			virtual void SetColor(float red, float green, float blue, float alpha);

			static Resource<Component> CreateComponent();
		};
	}
}