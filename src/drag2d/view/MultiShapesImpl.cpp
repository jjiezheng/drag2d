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

#include "MultiShapesImpl.h"

using namespace d2d;

MultiShapesImpl::MultiShapesImpl(wxWindow* wnd)
{
	m_wnd = wnd;
	m_shapeSelection = new ShapeSelection;
}

MultiShapesImpl::~MultiShapesImpl()
{
	m_shapeSelection->release();
}

IShape* MultiShapesImpl::queryShapeByPos(const Vector& pos) const
{
	IShape* result = NULL;
	traverseShapes(PointQueryVisitor(pos, &result), e_editable);
	return result;
}

void MultiShapesImpl::queryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const
{
	traverseShapes(RectQueryVisitor(rect, result), e_editable);
}

void MultiShapesImpl::removeShapeSelection()
{
	if (!m_shapeSelection->empty())
	{
		m_shapeSelection->traverse(RemoveSelectionVisitor(this));
		m_shapeSelection->clear();
		m_wnd->Refresh();
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::PointQueryVisitor::PointQueryVisitor(const Vector& pos, IShape** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void MultiShapesImpl::PointQueryVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	if (shape->isContain(m_pos))
	{
		*m_pResult = shape;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::RectQueryVisitor::RectQueryVisitor(const Rect& rect, std::vector<IShape*>& result)
	: m_rect(rect), m_result(result)
{
}

void MultiShapesImpl::RectQueryVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	if (shape->isIntersect(m_rect))
		m_result.push_back(shape);
	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::RemoveSelectionVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::RemoveSelectionVisitor::RemoveSelectionVisitor(MultiShapesImpl* shapesImpl)
{
	m_shapesImpl = shapesImpl;
}

void MultiShapesImpl::RemoveSelectionVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	m_shapesImpl->removeShape(static_cast<IShape*>(object));
	bFetchNext = true;
}
