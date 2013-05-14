/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#include "AABB.h"

#include "common/Math.h"

using namespace d2d;

AABB::AABB()
{
}

AABB::AABB(const AABB& aabb)
{
	m_rect = aabb.m_rect;	
}

AABB* AABB::clone()
{
	return new AABB(*this);
}

void AABB::loadFromTextFile(std::ifstream& fin)
{
	fin >> m_rect.xMin >> m_rect.xMax >> m_rect.yMin >> m_rect.yMax;
}

void AABB::storeToTextFile(std::ofstream& fout) const
{
	fout << m_rect.xMin << " " << m_rect.xMax << " "
		<< m_rect.yMin << " " << m_rect.yMax << '\n';
}

bool AABB::isContain(const Vector& pos) const
{
	return Math::isPointInAABB(pos, m_rect);
}

bool AABB::isIntersect(const Rect& rect) const
{
	return Math::isAABBIntersectAABB(m_rect, rect);
}

float AABB::area() const
{
	return m_rect.xLength() * m_rect.yLength();
}

float AABB::width() const
{
	return m_rect.xLength();
}

float AABB::height() const
{
	return m_rect.yLength();
}

Vector AABB::center() const
{
	return Vector(m_rect.xCenter(), m_rect.yCenter());
}

void AABB::getBoundPos(std::vector<Vector>& bound) const
{
	bound.clear();
	bound.push_back(Vector(m_rect.xMin, m_rect.yMin));
	bound.push_back(Vector(m_rect.xMax, m_rect.yMin));
	bound.push_back(Vector(m_rect.xMax, m_rect.yMax));
	bound.push_back(Vector(m_rect.xMin, m_rect.yMax));
}
