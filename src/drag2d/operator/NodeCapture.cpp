#include "NodeCapture.h"

#include "view/MultiShapesImpl.h"
#include "dataset/BezierShape.h"
#include "dataset/ChainShape.h"
#include "dataset/CircleShape.h"
#include "dataset/RectShape.h"
#include "common/Math.h"

namespace d2d
{

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
visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;
	if (BezierShape* bezier = dynamic_cast<BezierShape*>(object))
		bFetchNext = !visit(bezier);
	else if (ChainShape* chain = dynamic_cast<ChainShape*>(object))
		bFetchNext = !visit(chain);
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(object))
		bFetchNext = !visit(circle);
	else if (RectShape* rect = dynamic_cast<RectShape*>(object))
		bFetchNext = !visit(rect);
}

bool NodeCapture::RectQueryVisitor::
visit(BezierShape* bezier)
{
	// capture center
	const Rect& rect = bezier->getRect();
	if (Math::getDistance(Vector(rect.xCenter(), rect.yCenter()), m_pos) < m_tolerance)
	{
		m_result.shape = bezier;
		m_result.pos.setInvalid();
		return true;
	}

	// capture control points
	for (size_t i = 0; i < 4; ++i)
	{
		if (Math::getDistance(bezier->points[i], m_pos) < m_tolerance)
		{
			m_result.shape = bezier;
			m_result.pos = bezier->points[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(ChainShape* chain)
{
	// capture center
	const Rect& rect = chain->getRect();
	if (Math::getDistance(Vector(rect.xCenter(), rect.yCenter()), m_pos) < m_tolerance)
	{
		m_result.shape = chain;
		m_result.pos.setInvalid();
		return true;
	}

	// capture control points
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
} // d2d