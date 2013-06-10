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

#include "BezierShape.h"

#include "render/PrimitiveDraw.h"
#include "common/Math.h"

using namespace d2d;

BezierShape::BezierShape(const BezierShape& bezier)
{
	for (size_t i = 0; i < 4; ++i)
		points[i] = bezier.points[i];
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector points[4])
{
	for (size_t i = 0; i < 4; ++i)
		this->points[i] = points[i];
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector& start, const d2d::Vector& end)
{
	points[0] = start;
	points[3] = end;

	d2d::Vector mid = (start + end) * 0.5f;
	d2d::Vector offset = (end - start) * 0.5f;
	points[1] = mid + Math::rotateVectorRightAngle(offset, true);
	points[2] = mid + Math::rotateVectorRightAngle(offset, false);

	createCurve();
}

BezierShape::~BezierShape()
{

}

BezierShape* BezierShape::clone()
{
	return new BezierShape(*this);
}

bool BezierShape::isContain(const Vector& pos) const
{
	bool ret = false;
	for (size_t i = 0; i < 4; ++i)
	{
		if (Math::getDistance(pos, points[i]) < RADIUS)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void BezierShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	ChainShape::draw(color);

	for (size_t i = 0; i < 4; ++i)
		PrimitiveDraw::drawRect(points[i], (float)RADIUS, (float)RADIUS);
}

void BezierShape::createCurve()
{
	const size_t num = std::max(20, (int)(Math::getDistance(points[0], points[3]) / 10));
	float dt = 1.0f / (num - 1);
	std::vector<Vector> vertices(num);
	for (size_t i = 0; i < num; ++i)
		vertices[i] = pointOnCubicBezier(i * dt);
	setVertices(vertices);
}

d2d::Vector BezierShape::pointOnCubicBezier(float t)
{
	float   ax, bx, cx;
	float   ay, by, cy;
	float   tSquared, tCubed;
	d2d::Vector result;

	cx = 3.0f * (points[1].x - points[0].x);
	bx = 3.0f * (points[2].x - points[1].x) - cx;
	ax = points[3].x - points[0].x - cx - bx;

	cy = 3.0f * (points[1].y - points[0].y);
	by = 3.0f * (points[2].y - points[1].y) - cy;
	ay = points[3].y - points[0].y - cy - by;

	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + points[0].x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + points[0].y;

	return result;
}