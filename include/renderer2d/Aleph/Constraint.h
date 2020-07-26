#pragma once

#include"Core.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	namespace Aleph
	{
		class INFINITY_API Constraint
		{
			friend class Component;

		protected:
			enum class Orientation
			{
				HORIZONTAL,
				VERTICAL
			} m_orientation;

			float m_value;
			unsigned int m_window_width, m_window_height;
			float m_window_aspect_ratio;

			Constraint(Orientation orientation);

		public:
			virtual ~Constraint();

		private:
			void SetWindowSize(unsigned int width, unsigned int height, float aspect_ratio);

		protected:
			virtual void Update() = 0;
		};

		class INFINITY_API HorizontalConstraint : virtual public Constraint
		{
		protected:
			HorizontalConstraint();
	
		public:
			virtual ~HorizontalConstraint();
		};

		class INFINITY_API VerticalConstraint : virtual public Constraint
		{
		protected:
			VerticalConstraint();

		public:
			virtual ~VerticalConstraint();
		};

		class INFINITY_API PixelConstraint : public HorizontalConstraint, virtual public VerticalConstraint
		{
			friend class Component;

		private:
			unsigned int m_px;

		public:
			PixelConstraint(unsigned int px);
			virtual ~PixelConstraint();

		protected:
			void Update() override;

		public:
			static Resource<PixelConstraint> CreatePixelConstraint(unsigned int px);
		};
	}
}

namespace Infinity
{
	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::Constraint>;
	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::HorizontalConstraint>;
	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::VerticalConstraint>;
	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::PixelConstraint>;
}