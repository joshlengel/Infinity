#pragma once

#include"Core.h"

#include"math/Math.h"

namespace Infinity
{
	class INFINITY_API Camera
	{
	public:
		virtual const Mat4f &GetProjectionViewMatrix() const = 0;
	};

	class INFINITY_API OrthoCamera : public Camera
	{
	private:
		Mat4f m_view;

	public:
		Vec2f position;
		float roll, zoom;

		float min_zoom = 0.05f;
		float max_zoom = 2.0f;

		OrthoCamera(const Vec2f &position = { 0, 0 }, float roll = 0.0f, float zoom = 1.0f);
		~OrthoCamera();

		void Update(float aspect_ratio);

		const Mat4f &GetProjectionViewMatrix() const override;
	};
}