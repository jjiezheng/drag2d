/*
* Copyright (c) 2012-2013 Guang Zhu http://runnersoft.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef D2D_VECTOR_H
#define D2D_VECTOR_H

#include <float.h>
#include <math.h>
#include <iostream>

namespace d2d
{
	class Vector
	{
	public:
		Vector() : x(0), y(0) {}
		Vector(float x, float y) : x(x), y(y) {}
		Vector(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
		Vector(const Vector& p) : x(p.x), y(p.y) {}

		Vector& operator = (const Vector& p) {
			x = p.x;
			y = p.y;
			return *this;
		}

		void setInvalid() { x = y = FLT_MAX; }
		bool isValid() const { return x != FLT_MAX && y != FLT_MAX; }

		void set(float _x, float _y) { x = _x; y = _y; }

		bool operator != (const Vector& v) const
		{
			return x != v.x || y != v.y;
		}
		bool operator == (const Vector& v) const
		{
			return x == v.x && y == v.y;
		}

		void operator += (const Vector& v)
		{
			x += v.x; y += v.y;
		}

		void operator *= (float f)
		{
			x *= f; y *= f;
		}

		void operator /= (float f)
		{
			x /= f; y /= f;
		}

		Vector operator + (const Vector& v) const
		{
			return Vector(x + v.x, y + v.y);
		}

		Vector operator - (const Vector& v) const
		{
			return Vector(x - v.x, y - v.y);
		}

		Vector operator * (float scale) const
		{
			return Vector(x * scale, y * scale);
		}

		Vector operator / (float scale) const
		{
			return Vector(x / scale, y / scale);
		}

		Vector operator - () const
		{
			return Vector(-x, -y);
		}

		float length() const
		{
			return sqrt(x * x + y * y);
		}

		void normalize() 
		{
			const float len = length();
			x /= len;
			y /= len;
		}

		void turnLeft90Deg() 
		{
			const float tmp = x;
			x = -y;
			y = tmp;
		}

		void toInteger() 
		{
			x = floor(x + 0.5f);
			y = floor(y + 0.5f);
		}

		float x, y;

	}; // Vector

	//inline Vector operator + (const Vector& a, const Vector& b)
	//{
	//	return Vector(a.x + b.x, a.y + b.y);
	//}

	//inline Vector operator - (const Vector& a, const Vector& b)
	//{
	//	return Vector(a.x - b.x, a.y - b.y);
	//}

	//	inline Vector operator * (const Vector& v, float s)
	//	{
	//		return Vector(v.x * s, v.y * s);
	//	}

	inline float f2Cross(const Vector& a, const Vector& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	inline float f2Dot(const Vector& a, const Vector& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline std::istream& operator >> (std::istream& is, Vector& pos)
	{
		is >> pos.x >> pos.y;
		return is;
	}
	inline std::ostream& operator << (std::ostream& os, const Vector& pos)
	{
		os << pos.x << " " << pos.y;
		return os;
	}

	class VectorCmp
	{
	public:
		bool operator () (const Vector& p0, const Vector& p1) const {
			return p0.x < p1.x || p0.x == p1.x && p0.y < p1.y;
		}
	}; // VectorCmp

}

#endif // D2D_VECTOR_H
