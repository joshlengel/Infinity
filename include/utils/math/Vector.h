#pragma once

#include"Core.h"

#include<cmath>

namespace Infinity
{
	template <typename T> class Mat2;
	template <typename T> class Mat3;
	template <typename T> class Mat4;

	template <typename T>
	class INFINITY_API Vec2
	{
	public:
		union
		{
			T position[2];

			struct { T x, y; };
			struct { T r, g; };
		};

		Vec2(T x, T y): x(x), y(y) {}
		Vec2(const T position[2]): x(position[0]), y(position[1]) {}
		Vec2(): Vec2(0, 0) {}
		Vec2(const Vec2 &v): Vec2(v.x, v.y) {}
		~Vec2() {}

		Vec2 &operator=(const Vec2 &v)
		{
			x = v.x; y = v.y;

			return *this;
		}

		constexpr const T &operator[](unsigned int i) const { return position[i]; }
		constexpr T &operator[](unsigned int i) { return position[i]; }

		constexpr Vec2 Add(const Vec2 &v) const { return { x + v.x, y + v.y }; }
		constexpr Vec2 Sub(const Vec2 &v) const { return { x + v.x, y + v.y }; }
		constexpr Vec2 Scale(T scalar) const { return { x * scalar, y * scalar }; }
		constexpr Vec2 Mul(const Mat2<T> &m) const { return m.Mul(*this); }

		constexpr T Dot(const Vec2 &v) const { return x * v.x + y * v.y; }
		constexpr T LengthSquared() const { return x * x + y * y; }
		constexpr T Length() const { return (T)std::hypot(x, y); }

		constexpr Vec2 Normalize() const { return Scale(1 / Length()); }

		constexpr Vec2 operator+(const Vec2 &v) const { return Add(v); }
		constexpr Vec2 operator-(const Vec2 &v) const { return Sub(v); }
		constexpr Vec2 operator*(T scalar) const { return Scale(scalar); }
		constexpr Vec2 operator*(const Mat2<T> &m) const { return Mul(m); }
		constexpr Vec2 operator-() const { return { -x, -y }; }

		Vec2 &operator+=(const Vec2 &v) { return operator=(Add(v)); }
		Vec2 &operator-=(const Vec2 &v) { return operator=(Sub(v)); }
		Vec2 &operator*=(T scalar) { return operator=(Scale(scalar)); }
		Vec2 &operator*=(const Mat2<T> &m) { return operator=(Mul(m)); }

		bool operator==(const Vec2 &v) const { return x == v.x && y == v.y; }
	};

	template <typename T>
	class INFINITY_API Vec3
	{
	public:
		union
		{
			T position[3];

			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		Vec3(T x, T y, T z): x(x), y(y), z(z) {}
		Vec3(const T position[3]): x(position[0]), y(position[1]), z(position[2]) {}
		Vec3(): Vec3(0, 0, 0) {}
		Vec3(const Vec3 &v): Vec3(v.x, v.y, v.z) {}
		~Vec3() {}

		Vec3 &operator=(const Vec3 &v)
		{
			x = v.x; y = v.y; z = v.z;

			return *this;
		}

		constexpr const T &operator[](unsigned int i) const { return position[i]; }
		constexpr T &operator[](unsigned int i) { return position[i]; }

		constexpr Vec3 Add(const Vec3 &v) const { return { x + v.x, y + v.y, z + v.z }; }
		constexpr Vec3 Sub(const Vec3 &v) const { return { x + v.x, y + v.y, z + v.z }; }
		constexpr Vec3 Scale(T scalar) const { return { x * scalar, y * scalar, z * scalar }; }
		constexpr Vec3 Mul(const Mat3<T> &m) const { return m.Mul(*this); }

		constexpr T Dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
		constexpr T LengthSquared() const { return x * x + y * y + z * z; }
		constexpr T Length() const { return (T)std::sqrt(LengthSquared()); }

		constexpr Vec3 Normalize() const { return Scale(1 / Length()); }

		constexpr Vec3 operator+(const Vec3 &v) const { return Add(v); }
		constexpr Vec3 operator-(const Vec3 &v) const { return Sub(v); }
		constexpr Vec3 operator*(T scalar) const { return Scale(scalar); }
		constexpr Vec3 operator*(const Mat3<T> &m) const { return Mul(m); }
		constexpr Vec3 operator-() const { return { -x, -y, -z }; }

		Vec3 &operator+=(const Vec3 &v) { return operator=(Add(v)); }
		Vec3 &operator-=(const Vec3 &v) { return operator=(Sub(v)); }
		Vec3 &operator*=(T scalar) { return operator=(Scale(scalar)); }
		Vec3 &operator*=(const Mat3<T> &m) { return operator=(Mul(m)); }

		bool operator==(const Vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	};

	template <typename T>
	class INFINITY_API Vec4
	{
	public:
		union
		{
			T position[4];

			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};

		Vec4(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
		Vec4(const T position[4]): x(position[0]), y(position[1]), z(position[2]), w(position[3]) {}
		Vec4(): Vec4(0, 0, 0, 0) {}
		Vec4(const Vec4 &v): Vec4(v.x, v.y, v.z, v.w) {}
		~Vec4() {}

		Vec4 &operator=(const Vec4 &v)
		{
			x = v.x; y = v.y; z = v.z; w = v.w;

			return *this;
		}

		constexpr const T &operator[](unsigned int i) const { return position[i]; }
		constexpr T &operator[](unsigned int i) { return position[i]; }

		constexpr Vec4 Add(const Vec4 &v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
		constexpr Vec4 Sub(const Vec4 &v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
		constexpr Vec4 Scale(T scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
		constexpr Vec4 Mul(const Mat4<T> &m) const { return m.Mul(*this); }

		constexpr T Dot(const Vec4 &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
		constexpr T LengthSquared() const { return x * x + y * y + z * z + w * w; }
		constexpr T Length() const { return (T)std::sqrt(LengthSquared()); }

		constexpr Vec4 Normalize() const { return Scale(1 / Length()); }

		constexpr Vec4 operator+(const Vec4 &v) const { return Add(v); }
		constexpr Vec4 operator-(const Vec4 &v) const { return Sub(v); }
		constexpr Vec4 operator*(T scalar) const { return Scale(scalar); }
		constexpr Vec4 operator*(const Mat4<T> &m) const { return Mul(m); }
		constexpr Vec4 operator-() const { return { -x, -y, -z, -w }; }

		Vec4 &operator+=(const Vec4 &v) { return operator=(Add(v)); }
		Vec4 &operator-=(const Vec4 &v) { return operator=(Sub(v)); }
		Vec4 &operator*=(T scalar) { return operator=(Scale(scalar)); }
		Vec4 &operator*=(const Mat4<T> &m) { return operator=(Mul(m)); }

		bool operator==(const Vec4 &v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	};

	INFINITY_TEMPLATE template class INFINITY_API Vec2<int>;
	INFINITY_TEMPLATE template class INFINITY_API Vec3<int>;
	INFINITY_TEMPLATE template class INFINITY_API Vec4<int>;
	INFINITY_TEMPLATE template class INFINITY_API Vec2<long>;
	INFINITY_TEMPLATE template class INFINITY_API Vec3<long>;
	INFINITY_TEMPLATE template class INFINITY_API Vec4<long>;
	INFINITY_TEMPLATE template class INFINITY_API Vec2<float>;
	INFINITY_TEMPLATE template class INFINITY_API Vec3<float>;
	INFINITY_TEMPLATE template class INFINITY_API Vec4<float>;
	INFINITY_TEMPLATE template class INFINITY_API Vec2<double>;
	INFINITY_TEMPLATE template class INFINITY_API Vec3<double>;
	INFINITY_TEMPLATE template class INFINITY_API Vec4<double>;

	typedef Vec2<int> Vec2i;
	typedef Vec3<int> Vec3i;
	typedef Vec4<int> Vec4i;
	typedef Vec2<long> Vec2l;
	typedef Vec3<long> Vec3l;
	typedef Vec4<long> Vec4l;
	typedef Vec2<float> Vec2f;
	typedef Vec3<float> Vec3f;
	typedef Vec4<float> Vec4f;
	typedef Vec2<double> Vec2d;
	typedef Vec3<double> Vec3d;
	typedef Vec4<double> Vec4d;
}