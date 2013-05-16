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

#ifndef D2D_RECT_H
#define D2D_RECT_H

#include "common/Vector.h"

namespace d2d
{
	class Rect
	{
	public:
		float xMin, xMax;
		float yMin, yMax;

		Rect() { makeInfinite(); }
		Rect(const Vector& p0, const Vector& p1) {
			xMin = std::min(p0.x, p1.x);
			yMin = std::min(p0.y, p1.y);
			xMax = std::max(p0.x, p1.x);
			yMax = std::max(p0.y, p1.y);
		}
		Rect(const Vector& center, float hWidth, float hHeight) {
			xMin = center.x - hWidth;
			xMax = center.x + hWidth;
			yMin = center.y - hHeight;
			yMax = center.y + hHeight;
		}

		const Rect& operator = (const Rect& aabb)
		{
			xMin = aabb.xMin;
			xMax = aabb.xMax;
			yMin = aabb.yMin;
			yMax = aabb.yMax;
			return *this;
		}

		void operator *= (float scale)
		{
			const float cx = xCenter(),
				cy = yCenter();
			const float dx = xLength() * scale * 0.5f,
				dy = yLength() * scale * 0.5f;
			xMin = cx - dx;
			xMax = cx + dx;
			yMin = cy - dy;
			yMax = cy + dy;
		}

		bool isValid() const {
			return xMin != FLT_MAX && yMin != FLT_MAX
				&& xMax != - FLT_MAX && yMax != - FLT_MAX;
		}
		void makeInfinite() {
			xMin = yMin = FLT_MAX;
			xMax = yMax = - FLT_MAX;
		}

		float xLength() const { return xMax - xMin; }
		float yLength() const { return yMax - yMin; }

		float xCenter() const { return (xMin + xMax) * 0.5f; }
		float yCenter() const { return (yMin + yMax) * 0.5f; }

		void translate(const Vector& offset)
		{
			xMin += offset.x;
			xMax += offset.x;
			yMin += offset.y;
			yMax += offset.y;
		}

		void combine(const Vector& pos)
		{
			if (pos.x < xMin) xMin = pos.x;
			if (pos.x > xMax) xMax = pos.x;
			if (pos.y < yMin) yMin = pos.y;
			if (pos.y > yMax) yMax = pos.y;
		}
		void combine(const Rect& aabb)
		{
			if (aabb.xMin < xMin) xMin = aabb.xMin;
			if (aabb.xMax > xMax) xMax = aabb.xMax;
			if (aabb.yMin < yMin) yMin = aabb.yMin;
			if (aabb.yMax > yMax) yMax = aabb.yMax;
		}

	}; // Rect
}

#endif // D2D_RECT_H
