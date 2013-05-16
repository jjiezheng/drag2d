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

#include "CurveShape.h"

#include "render/PrimitiveDraw.h"
#include "common/Settings.h"
#include "algorithms/CosineSmooth.h"
#include "algorithms/DouglasPeucker.h"

using namespace d2d;

#define SIMPLIFY

const float CurveShape::SAMPLING_WIDTH = 10.0f;

CurveShape::CurveShape(const CurveShape& curve)
	: ChainShape(curve)
{
	m_midPoints = curve.m_midPoints;
}

CurveShape::CurveShape(const std::vector<Vector>& vertices)
	: ChainShape(vertices, false)
{
	setMidPoints();
}

CurveShape::~CurveShape()
{
	
}

CurveShape* CurveShape::clone()
{
	return new CurveShape(*this);	
}

void CurveShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	if (m_vertices.empty()) return;

	PrimitiveDraw::resetColorAndTexture();

	PrimitiveDraw::drawPolyline(m_midPoints, color, m_isLoop);
	if (Settings::ctlPosSize != 0)
		PrimitiveDraw::drawCircles(m_vertices, Settings::ctlPosSize, Colorf(0.4f, 0.8f, 0.4f));
	if (Settings::ctlPosSize != 0)
		PrimitiveDraw::drawCircles(m_midPoints, Settings::ctlPosSize * 0.5f, Colorf(0.8f, 0.8f, 0.4f));
}

void CurveShape::insertVertices(size_t index, const Vector& pos)
{
	ChainShape::insertVertices(index, pos);
	setMidPoints();
}

void CurveShape::removeVertices(const Vector& pos)
{
	ChainShape::removeVertices(pos);
	setMidPoints();
}

void CurveShape::changeVertices(const Vector& from, const Vector& to)
{
	ChainShape::changeVertices(from, to);
	setMidPoints();
}

void CurveShape::refresh()
{
//	createBody();
}

void CurveShape::setMidPoints()
{
	m_midPoints.clear();

	if (m_vertices.size() <= 1) return;

	std::vector<Vector> smooth;
	CosineSmooth::implement(m_vertices, SAMPLING_WIDTH, smooth);

#ifdef SIMPLIFY
	DouglasPeucker::implement(smooth, 0.75f, m_midPoints);
#endif
}
