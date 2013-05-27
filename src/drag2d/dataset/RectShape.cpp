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

#include "RectShape.h"

#include "common/Math.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

RectShape::RectShape()
	: m_rect(Vector(0, 0), 1, 1)
{
}

RectShape::RectShape(const RectShape& rect)
	: m_rect(rect.m_rect)
{
}

RectShape::RectShape(const Vector& p0, const Vector& p1)
	: m_rect(p0, p1)
{
}

RectShape::RectShape(const Vector& center, float hWidth, float hHeight)
	: m_rect(center, hWidth, hHeight)
{
}

RectShape* RectShape::clone()
{
	return new RectShape(*this);
}

void RectShape::loadFromTextFile(std::ifstream& fin)
{
}

void RectShape::storeToTextFile(std::ofstream& fout) const
{
}


bool RectShape::isContain(const Vector& pos) const
{
	return Math::isPointInRect(pos, m_rect);
}

bool RectShape::isIntersect(const Rect& rect) const
{
	return Math::isRectIntersectRect(rect, m_rect);
}

void RectShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	PrimitiveDraw::drawRect(Vector(m_rect.xMin, m_rect.yMin), 
		Vector(m_rect.xMax, m_rect.yMax), false, 2, color);
}