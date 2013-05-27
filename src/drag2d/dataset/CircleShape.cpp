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

#include "CircleShape.h"

#include "common/Math.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

CircleShape::CircleShape()
	: center(0, 0)
	, radius(1)
	, m_rect(center, radius, radius)
{
}

CircleShape::CircleShape(const CircleShape& circle)
{
	center = circle.center;
	radius = circle.radius;
	m_rect = circle.m_rect;
}

CircleShape::CircleShape(const Vector& center, float radius)
	: center(center)
	, radius(radius)
	, m_rect(center, radius, radius)
{
}

CircleShape* CircleShape::clone()
{
	return new CircleShape(*this);
}

void CircleShape::loadFromTextFile(std::ifstream& fin)
{
}

void CircleShape::storeToTextFile(std::ofstream& fout) const
{
}

bool CircleShape::isContain(const Vector& pos) const
{
	return Math::getDistance(center, pos) < QUERY_ACCURACY;
}

bool CircleShape::isIntersect(const Rect& rect) const
{
	const float cx = (rect.xMin + rect.xMax) * 0.5f,
		cy = (rect.yMin + rect.yMax) * 0.5f;

	const float dis = Math::getDistance(center, Vector(cx, cy));
	if (dis > Vector(rect.xMax - cx, rect.yMax - cy).length() + radius)
		return false;

	if (Math::isPointInCircle(Vector(rect.xMin, rect.yMin), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMax, rect.yMin), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMax, rect.yMax), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMin, rect.yMax), center, radius))
		return true;
	if (Math::isPointInRect(center, rect))
		return true;

	if (center.x > rect.xMin && center.x < rect.xMax)
	{
		if (dis < rect.yMax - cy + radius)
			return true;
	}
	if (center.y > rect.yMin && center.y < rect.yMax)
	{
		if (dis < rect.xMax - cx + radius)
			return true;
	}

	return false;
}

void CircleShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	PrimitiveDraw::drawCircle(center, radius, false, 3, color, 32);
}
