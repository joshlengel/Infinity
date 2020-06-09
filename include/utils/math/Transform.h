#pragma once

#include"Core.h"

#include"Vector.h"
#include"Matrix.h"

namespace Infinity
{
	template <typename T>
	INFINITY_API inline Mat4<T> MakeTranslation(T x, T y, T z)
	{
		return
		{
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		};
	}

	template <typename T> INFINITY_API inline Mat4<T> MakeTranslation(const Vec3<T> &t) { return MakeTranslation(t.x, t.y, t.z); }

	template <typename T>
	INFINITY_API Mat4<T> inline MakeAxisRotation(T ax, T ay, T az, T angle)
	{
		sina = (T)sin(angle);
		cosa = (T)cos(angle);
		cos1a = 1 - cosa;

		return
		{
			     cosa + ax * ax * cos1a, ax * ay * cos1a - az * sina, ax * az * cos1a + ay * sina, 0,
			ay * ax * cos1a + az * sina,      cosa + ay * ay * cos1a, ay * az * cos1a - ax * sina, 0,
			az * ax * cos1a - ay * sina, az * ay * cos1a + ax * sina,      cosa + az * az * cos1a, 0,
			                          0,                           0,                           0, 1
		};
	}

	template <typename T> INFINITY_API inline Mat4<T> MakeAxisRotation(const Vec3<T> &axis, T angle) { return MakeAxisRotation(axis.x, axis.y, axis.z, angle); }

	template <typename T>
	INFINITY_API Mat4<T> inline MakeRollPitchYawRotation(T pitch, T yaw, T roll)
	{
		T sinp = (T)sin(pitch);
		T cosp = (T)cos(pitch);

		Mat4<T> p =
		{
			1,     0,    0, 0,
			0,  cosp, sinp, 0,
			0, -sinp, cosp, 0,
			0,     0,    0, 1
		};

		T siny = (T)sin(yaw);
		T cosy = (T)cos(yaw);

		Mat4<T> y =
		{
			 cosy, 0, siny, 0,
			    0, 1,    0, 0,
			-siny, 0, cosy, 0,
			    0, 0,    0, 1
		};

		T sinr = (T)sin(roll);
		T cosr = (T)cos(roll);

		Mat4<T> r =
		{
			 cosr, sinr, 0, 0,
			-sinr, cosr, 0, 0,
			    0,    0, 1, 0,
			    0,    0, 0, 1
		};

		return r * p * y;
	}

	template <typename T> INFINITY_API inline Mat4<T> MakeRollPitchYawRotation(const Vec3<T> &pyr) { return MakeRollPitchYawRotation(pyr.x, pyr.y, pyr.z); }

	template <typename T>
	INFINITY_API Mat4<T> inline MakeScale(T sx, T sy, T sz)
	{
		return
		{
			sx,   0,  0, 0,
			 0,  sy,  0, 0,
			 0,   0, sz, 0,
			 0,   0,  0, 1
		};
	}

	template <typename T> INFINITY_API inline Mat4<T> MakeScale(const Vec3<T> &s) { return MakeScale(s.x, s.y, s.z); }

	template <typename T>
	INFINITY_API inline Mat4<T> MakePerspectiveProjection(T fovy, T aspect_ratio, T clip_near, T clip_far)
	{
		T tan_half_fov = (T)tan(fovy / 2);
		T inv_clip_diff = 1 / (clip_far - clip_near);

		return
		{
			1 / (tan_half_fov * aspect_ratio),                0,                                      0,                                         0,
			                                0, 1 / tan_half_fov,                                      0,                                         0,
			                                0,                0, (clip_near + clip_far) * inv_clip_diff, -2 * clip_near * clip_far * inv_clip_diff,
			                                0,                0,                                      1,                                         0
		};
	}

	template <typename T>
	INFINITY_API inline Mat4<T> MakeOrthoProjection(T left, T right, T bottom, T top, T near, T far)
	{
		return MakeScale(2 / (right - left), 2 / (top - bottom), 2 / (far - near)) * MakeTranslation(-(left + right) / 2, -(bottom + top) / 2, -(near + far) / 2);
	}

	template <typename T> INFINITY_API inline Mat4<T> Translate(const Mat4<T> &m, T x, T y, T z) { return MakeTranslation(x, y, z) * m; }
	template <typename T> INFINITY_API inline Mat4<T> Translate(const Mat4<T> &m, const Vec3<T> &t) { return MakeTranslation(t) * m; }

	template <typename T> INFINITY_API inline Mat4<T> AxisRotate(const Mat4<T> &m, T ax, T ay, T az, T angle) { return MakeAxisRotation(ax, ay, az, angle) * m; }
	template <typename T> INFINITY_API inline Mat4<T> AxisRotate(const Mat4<T> &m, const Vec3<T> &axis, T angle) { return MakeAxisRotation(axis, angle) * m; }

	template <typename T> INFINITY_API inline Mat4<T> RollPitchYawRotate(const Mat4<T> &m, T pitch, T yaw, T roll) { return MakeRollPitchYawRotation(pitch, yaw, roll) * m; }
	template <typename T> INFINITY_API inline Mat4<T> RollPitchYawRotate(const Mat4<T> &m, const Vec3<T> &pyr) { return MakeRollPitchYawRotation(pyr) * m; }

	template <typename T> INFINITY_API inline Mat4<T> Scale(const Mat4<T> &m, T sx, T sy, T sz) { return MakeScale(sx, sy, sz) * m; }
	template <typename T> INFINITY_API inline Mat4<T> Scale(const Mat4<T> &m, const Vec3<T> &s) { return MakeScale(s) * m; }

	template <typename T> INFINITY_API inline Mat4<T> ProjectPrespective(const Mat4<T> &m, T fovy, T aspect_ratio, T clip_near, T clip_far) { return MakePerspectiveProjection(fovy, aspect_ratio, clip_near, clip_far) * m; }
	template <typename T> INFINITY_API inline Mat4<T> ProjectOrtho(const Mat4<T> &m, T left, T right, T bottom, T top, T near, T far) { return MakeOrthoProjection(left, right, bottom, top, near, far) * m; }
}