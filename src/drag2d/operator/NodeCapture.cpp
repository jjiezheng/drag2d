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

#include "NodeCapture.h"

#include "view/MultiShapesImpl.h"
#include "dataset/ChainShape.h"
#include "dataset/CircleShape.h"
#include "dataset/RectShape.h"
#include "common/Math.h"

using namespace d2d;

NodeCapture::NodeCapture(MultiShapesImpl* shapesImpl, int tol)
	: m_shapesImpl(shapesImpl)
	, m_tol(tol)
{
}

void NodeCapture::captureEditable(const Vector& pos, NodeAddr& result)
{
	m_shapesImpl->traverseShapes(RectQueryVisitor(pos, m_tol, result), e_editable);
}

void NodeCapture::captureSelectable(const Vector& pos, NodeAddr& result)
{
	m_shapesImpl->traverseShapes(RectQueryVisitor(pos, m_tol, result), e_selectable);
}

//////////////////////////////////////////////////////////////////////////
// class NodeCapture::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

NodeCapture::RectQueryVisitor::
RectQueryVisitor(const Vector& pos, float tolerance, NodeAddr& result)
	: m_pos(pos)
	, m_tolerance(tolerance)
	, m_rect(pos, tolerance, tolerance)
	, m_result(result)
{
	result.shape = NULL;
}

void NodeCapture::RectQueryVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	bFetchNext = true;
	if (ChainShape* chain = dynamic_cast<ChainShape*>(object))
		bFetchNext = !visit(chain);
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(object))
		bFetchNext = !visit(circle);
	else if (RectShape* rect = dynamic_cast<RectShape*>(object))
		bFetchNext = !visit(rect);
}

bool NodeCapture::RectQueryVisitor::
visit(ChainShape* chain)
{
	if (!Math::isRectIntersectRect(m_rect, chain->getRect()))
		return false;

	if (!chain->isIntersect(m_rect)) 
		return false;

	const std::vector<Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		if (Math::getDistance(vertices[i], m_pos) < m_tolerance)
		{
			m_result.shape = chain;
			m_result.pos = vertices[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(CircleShape* circle)
{
	const float dis = Math::getDistance(circle->center, m_pos);

	// capture center
	if (dis < m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos = circle->center;
		return true;
	}
	// capture ring
	else if (dis < circle->radius + m_tolerance 
		&& dis > circle->radius - m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos.setInvalid();
		return true;
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(RectShape* rect)
{
	// capture center
	if (Math::getDistance(m_pos, Vector(rect->m_rect.xCenter(), rect->m_rect.yCenter())) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos.setInvalid();
		return true;
	}
	// capture edge
	else if (Math::getDistance(m_pos, Vector(rect->m_rect.xMin, rect->m_rect.yMin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = Vector(rect->m_rect.xMin, rect->m_rect.yMin);
		return true;
	}
	else if (Math::getDistance(m_pos, Vector(rect->m_rect.xMin, rect->m_rect.yMax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = Vector(rect->m_rect.xMin, rect->m_rect.yMax);
		return true;
	}
	else if (Math::getDistance(m_pos, Vector(rect->m_rect.xMax, rect->m_rect.yMax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = Vector(rect->m_rect.xMax, rect->m_rect.yMax);
		return true;
	}
	else if (Math::getDistance(m_pos, Vector(rect->m_rect.xMax, rect->m_rect.yMin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = Vector(rect->m_rect.xMax, rect->m_rect.yMin);
		return true;
	}
	return false;
}