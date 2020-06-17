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

	class INFINITY_API PerspectiveCamera : public Camera
	{
	private:
		Mat4f m_view;

		float m_cache_pitch, m_cache_sin_pitch, m_cache_cos_pitch;
		float m_cache_yaw, m_cache_sin_yaw, m_cache_cos_yaw;
		
	public:
		Vec3f position;
		float roll, pitch, yaw;

		float clip_near = 0.1f;
		float clip_far = 100.0f;

		float fov = 3.14159f * 0.25f;

		PerspectiveCamera(const Vec3f &position = { 0.0f, 0.0f, 0.0f }, float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f);
		~PerspectiveCamera();

		void Update(float aspect_ratio);
		const Mat4f &GetProjectionViewMatrix() const override;

		void MoveForward(float speed);
		void MoveBackward(float speed);
		void MoveLeft(float speed);
		void MoveRight(float speed);
		void MoveUp(float speed);
		void MoveDown(float speed);

		void MoveForwardXZ(float speed);
		void MoveBackwardXZ(float speed);
		void MoveUpXZ(float speed);
		void MoveDownXZ(float speed);

	private:
		void UpdateCache();
	};
}