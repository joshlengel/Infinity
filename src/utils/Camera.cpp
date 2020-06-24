#include"InfinityPCH.h"

#include"utils/Camera.h"

namespace Infinity
{
	OrthoCamera::OrthoCamera(const Vec2f &position, float roll, float zoom):
		position(position),
		roll(roll),
		m_cache_roll(roll),
		m_cache_sin_roll(sin(roll)),
		m_cache_cos_roll(cos(roll)),
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

	void OrthoCamera::UpdateCache()
	{
		if (m_cache_roll != roll)
		{
			m_cache_roll = roll;
			m_cache_sin_roll = sin(roll);
			m_cache_cos_roll = cos(roll);
		}
	}

	void OrthoCamera::MoveForward(float speed)
	{
		UpdateCache();

		position.x += m_cache_sin_roll * speed;
		position.y += m_cache_cos_roll * speed;
	}

	void OrthoCamera::MoveBackward(float speed)
	{
		UpdateCache();

		position.x -= m_cache_sin_roll * speed;
		position.y -= m_cache_cos_roll * speed;
	}

	void OrthoCamera::MoveLeft(float speed)
	{
		UpdateCache();

		position.x -= m_cache_cos_roll * speed;
		position.y += m_cache_sin_roll * speed;
	}

	void OrthoCamera::MoveRight(float speed)
	{
		UpdateCache();

		position.x += m_cache_cos_roll * speed;
		position.y -= m_cache_sin_roll * speed;
	}

	PerspectiveCamera::PerspectiveCamera(const Vec3f &position, float roll, float pitch, float yaw):
		position(position),
		roll(roll), pitch(pitch), yaw(yaw),
		m_view(),
		m_cache_pitch(pitch),
		m_cache_sin_pitch(sin(pitch)),
		m_cache_cos_pitch(cos(pitch)),
		m_cache_yaw(yaw),
		m_cache_sin_yaw(sin(yaw)),
		m_cache_cos_yaw(cos(yaw))
	{}

	PerspectiveCamera::~PerspectiveCamera()
	{}

	void PerspectiveCamera::Update(float aspect_ratio)
	{
		m_view = MakePerspectiveProjection(fov, aspect_ratio, clip_near, clip_far) * MakeRollPitchYawRotation(-pitch, -yaw, -roll) * MakeTranslation(-position);
	}

	const Mat4f &PerspectiveCamera::GetProjectionViewMatrix() const { return m_view; }

	void PerspectiveCamera::UpdateCache()
	{
		if (m_cache_pitch != pitch)
		{
			m_cache_pitch = pitch;
			m_cache_sin_pitch = sin(pitch);
			m_cache_cos_pitch = cos(pitch);
		}

		if (m_cache_yaw != yaw)
		{
			m_cache_yaw = yaw;
			m_cache_sin_yaw = sin(yaw);
			m_cache_cos_yaw = cos(yaw);
		}
	}

	void PerspectiveCamera::MoveForward(float speed)
	{
		UpdateCache();
		position.x += speed * m_cache_sin_yaw * m_cache_cos_pitch;
		position.y += speed * m_cache_sin_pitch;
		position.z += speed * m_cache_cos_yaw * m_cache_cos_pitch;
	}

	void PerspectiveCamera::MoveBackward(float speed)
	{
		UpdateCache();
		position.x -= speed * m_cache_sin_yaw * m_cache_cos_pitch;
		position.y -= speed * m_cache_sin_pitch;
		position.z -= speed * m_cache_cos_yaw * m_cache_cos_pitch;
	}

	void PerspectiveCamera::MoveUp(float speed)
	{
		UpdateCache();
		position.x -= speed * m_cache_sin_pitch * m_cache_sin_yaw;
		position.y += speed * m_cache_cos_pitch;
		position.z -= speed * m_cache_sin_pitch * m_cache_cos_yaw;
	}

	void PerspectiveCamera::MoveDown(float speed)
	{
		UpdateCache();
		position.x += speed * m_cache_sin_pitch * m_cache_sin_yaw;
		position.y -= speed * m_cache_cos_pitch;
		position.z += speed * m_cache_sin_pitch * m_cache_cos_yaw;
	}

	void PerspectiveCamera::MoveForwardXZ(float speed)
	{
		UpdateCache();
		position.x += speed * m_cache_sin_yaw;
		position.z += speed * m_cache_cos_yaw;
	}

	void PerspectiveCamera::MoveBackwardXZ(float speed)
	{
		UpdateCache();
		position.x -= speed * m_cache_sin_yaw;
		position.z -= speed * m_cache_cos_yaw;
	}

	void PerspectiveCamera::MoveLeft(float speed)
	{
		UpdateCache();
		position.x -= speed * m_cache_cos_yaw;
		position.z += speed * m_cache_sin_yaw;
	}

	void PerspectiveCamera::MoveRight(float speed)
	{
		UpdateCache();
		position.x += speed * m_cache_cos_yaw;
		position.z -= speed * m_cache_sin_yaw;
	}

	void PerspectiveCamera::MoveUpXZ(float speed)
	{
		UpdateCache();
		position.y += speed;
	}

	void PerspectiveCamera::MoveDownXZ(float speed)
	{
		UpdateCache();
		position.y -= speed;
	}
}