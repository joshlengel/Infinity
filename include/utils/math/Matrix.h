#pragma once

#include "Core.h"

#include"Vector.h"

#include<memory>

namespace Infinity
{
	template <typename T>
	class INFINITY_API Mat2
	{
	public:
		T columns[2][2];

		Mat2(T m00, T m10, T m01, T m11): columns{ { m00, m01 }, { m10, m11 } } {}
		Mat2(): Mat2(1, 0, 0, 1) {}
		Mat2(const Mat2 &m): Mat2(m[0][0], m[1][0], m[0][1], m[1][1]) {}
		~Mat2() {}

		Mat2 &operator=(const Mat2 &m)
		{
			memcpy(columns, m.columns, sizeof(columns));

			return *this;
		}

		constexpr const T *operator[](unsigned int i) const { return columns[i]; }
		constexpr T *operator[](unsigned int i) { return columns[i]; }

		
		constexpr Vec2<T> Mul(const Vec2<T> &v) const
		{
			return
			{
				columns[0][0] * v.x + columns[1][0] * v.y,
				columns[0][1] * v.x + columns[1][1] * v.y
			};
		}

		constexpr Mat2 Mul(const Mat2 &m) const
		{
			Mat2 res;
			Vec2<T> r0 = Mul(Vec2<T>(m[0]));
			Vec2<T> r1 = Mul(Vec2<T>(m[1]));

			memcpy(res[0], r0.position, sizeof(T[2]));
			memcpy(res[1], r1.position, sizeof(T[2]));

			return res;
		}

		constexpr Vec2<T> operator*(const Vec2<T> &v) const { return Mul(v); }
		constexpr Mat2 operator*(const Mat2 &m) const { return Mul(m); }

		constexpr bool operator==(const Mat2 &m) const { return columns[0] == m[0] && columns[1] == m[1]; }
	};

	template <typename T>
	class INFINITY_API Mat3
	{
	public:
		T columns[3][3];

		Mat3(T m00, T m10, T m20, T m01, T m11, T m21, T m02, T m12, T m22): columns{ { m00, m01, m02 }, { m10, m11, m12 }, { m20, m21, m22 } } {}
		Mat3(): Mat3(1, 0, 0, 0, 1, 0, 0, 0, 1) {}
		Mat3(const Mat3 &m): Mat3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]) {}
		~Mat3() {}

		
		Mat3 &operator=(const Mat3 &m)
		{
			memcpy(columns, m.columns, sizeof(columns));

			return *this;
		}

		constexpr const T *operator[](unsigned int i) const { return columns[i]; }
		constexpr T *operator[](unsigned int i) { return columns[i]; }

		
		constexpr Vec3<T> Mul(const Vec3<T> &v) const
		{
			return
			{
				columns[0][0] * v.x + columns[1][0] * v.y + columns[2][0] * v.z,
				columns[0][1] * v.x + columns[1][1] * v.y + columns[2][1] * v.z,
				columns[0][2] * v.x + columns[1][2] * v.y + columns[2][2] * v.z
			};
		}

		constexpr Mat3 Mul(const Mat3 &m) const
		{
			Mat3 res;
			Vec3<T> r0 = Mul(Vec3<T>(m[0]));
			Vec3<T> r1 = Mul(Vec3<T>(m[1]));
			Vec3<T> r2 = Mul(Vec3<T>(m[2]));

			memcpy(res[0], r0.position, sizeof(T[3]));
			memcpy(res[1], r1.position, sizeof(T[3]));
			memcpy(res[2], r2.position, sizeof(T[3]));

			return res;
		}

		constexpr Vec3<T> operator*(const Vec3<T> &v) const { return Mul(v); }
		constexpr Mat3 operator*(const Mat3 &m) const { return Mul(m); }

		constexpr bool operator==(const Mat3 &m) const { return columns[0] == m[0] && columns[1] == m[1] && columns[2] == m[2]; }
	};

	template <typename T>
	class INFINITY_API Mat4
	{
	public:
		T columns[4][4];

		Mat4(T m00, T m10, T m20, T m30, T m01, T m11, T m21, T m31, T m02, T m12, T m22, T m32, T m03, T m13, T m23, T m33): columns{ { m00, m01, m02, m03 }, { m10, m11, m12, m13 }, { m20, m21, m22, m23 }, { m30, m31, m32, m33 } } {}
		Mat4(): Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) {}
		Mat4(const Mat4 &m): Mat4(m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2], m[0][3], m[1][3], m[2][3], m[3][3]) {}
		~Mat4() {}

		
		Mat4 &operator=(const Mat4 &m)
		{
			memcpy(columns, m.columns, sizeof(columns));
			
			return *this;
		}

		constexpr const T *operator[](unsigned int i) const { return columns[i]; }
		constexpr T *operator[](unsigned int i) { return columns[i]; }

		
		constexpr Vec4<T> Mul(const Vec4<T> &v) const
		{
			return
			{
				columns[0][0] * v.x + columns[1][0] * v.y + columns[2][0] * v.z + columns[3][0] * v.w,
				columns[0][1] * v.x + columns[1][1] * v.y + columns[2][1] * v.z + columns[3][1] * v.w,
				columns[0][2] * v.x + columns[1][2] * v.y + columns[2][2] * v.z + columns[3][2] * v.w,
				columns[0][3] * v.x + columns[1][3] * v.y + columns[2][3] * v.z + columns[3][3] * v.w
			};
		}

		constexpr Mat4 Mul(const Mat4 &m) const
		{
			Mat4 res;
			Vec4<T> r0 = Mul(Vec4<T>(m[0]));
			Vec4<T> r1 = Mul(Vec4<T>(m[1]));
			Vec4<T> r2 = Mul(Vec4<T>(m[2]));
			Vec4<T> r3 = Mul(Vec4<T>(m[3]));

			memcpy(res[0], r0.position, sizeof(T[4]));
			memcpy(res[1], r1.position, sizeof(T[4]));
			memcpy(res[2], r2.position, sizeof(T[4]));
			memcpy(res[3], r3.position, sizeof(T[4]));

			return res;
		}

		constexpr Vec4<T> operator*(const Vec4<T> &v) const { return Mul(v); }
		constexpr Mat4 operator*(const Mat4 &m) const { return Mul(m); }

		constexpr bool operator==(const Mat4 &m) const { return columns[0] == m[0] && columns[1] == m[1] && columns[2] == m[2] && columns[3] == m[3]; }
	};

	INFINITY_TEMPLATE template class INFINITY_API Mat2<int>;
	INFINITY_TEMPLATE template class INFINITY_API Mat3<int>;
	INFINITY_TEMPLATE template class INFINITY_API Mat4<int>;
	INFINITY_TEMPLATE template class INFINITY_API Mat2<long>;
	INFINITY_TEMPLATE template class INFINITY_API Mat3<long>;
	INFINITY_TEMPLATE template class INFINITY_API Mat4<long>;
	INFINITY_TEMPLATE template class INFINITY_API Mat2<float>;
	INFINITY_TEMPLATE template class INFINITY_API Mat3<float>;
	INFINITY_TEMPLATE template class INFINITY_API Mat4<float>;
	INFINITY_TEMPLATE template class INFINITY_API Mat2<double>;
	INFINITY_TEMPLATE template class INFINITY_API Mat3<double>;
	INFINITY_TEMPLATE template class INFINITY_API Mat4<double>;

	typedef Mat2<int> Mat2i;
	typedef Mat3<int> Mat3i;
	typedef Mat4<int> Mat4i;
	typedef Mat2<long> Mat2l;
	typedef Mat3<long> Mat3l;
	typedef Mat4<long> Mat4l;
	typedef Mat2<float> Mat2f;
	typedef Mat3<float> Mat3f;
	typedef Mat4<float> Mat4f;
	typedef Mat2<double> Mat2d;
	typedef Mat3<double> Mat3d;
	typedef Mat4<double> Mat4d;
}