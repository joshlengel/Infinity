#include"utils/Camera.h"

namespace Infinity
{
	OrthoCamera::OrthoCamera(const Vec2f &position, float roll, float zoom):
		position(position),
		roll(roll),
		zoom(zoom)
	{}

	OrthoCamera::~OrthoCamera() {}

	void OrthoCamera::Update(float aspect_ratio)
	{
		zoom = zoom < min_zoom? min_zoom : zoom > max_zoom? max_zoom : zoom;

		float sinr = sin(roll);
		float cosr = cos(roll);

		Mat4f rotate =
		{
			cosr, -sinr, 0, 0,
			sinr,  cosr, 0, 0,
			   0,     0, 1, 0,
			   0,     0, 0, 1
		};

		float width = 1.0f / zoom;

		m_view = (aspect_ratio >= 1? MakeScale(width / aspect_ratio, width, 1.0f) : MakeScale(width, width * aspect_ratio, 1.0f)) * rotate * MakeTranslation(-position.x, -position.y, 0.0f);
	}

	const Mat4f &OrthoCamera::GetProjectionViewMatrix() const { return m_view; }
}