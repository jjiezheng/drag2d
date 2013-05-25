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

#ifndef D2D_EDIT_POLYLINE_OP_CPP
#define D2D_EDIT_POLYLINE_OP_CPP

#include "EditPolylineOP.h"

#include "common/Math.h"
#include "render/PrimitiveDraw.h"

//using namespace d2d;

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::
EditPolylineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			   NodeCaptureCMPT<EditPolylineOP>* cmpt)
	: TBase(editPanel, shapesImpl)
{
	m_shapesImpl = shapesImpl;

	m_cmpt = cmpt;

	m_selectOP = new TSelected(editPanel, shapesImpl, cmpt);
	m_lastLeftDownPos.setInvalid();
	m_bSelectOpen = false;
}

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::
~EditPolylineOP() 
{
	delete m_selectOP;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onKeyDown(int keyCode) 
{
	return m_selectOP->onKeyDown(keyCode);
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseLeftDown(int x, int y) 
{
	bool bNotDeliver = false;

	if (shouldFixMousePos())
		bNotDeliver = TBase::onMouseLeftDown(x, y);
	else
	{
		int tolerance;
		if (m_cmpt)
			tolerance = m_cmpt->getNodeCaptureDistance();
		else
			tolerance = 0;
		if (tolerance != 0)
		{	
			NodeCapture capture(m_shapesImpl, tolerance);
			Vector pos = m_editPanel->transPosScreenToProject(x, y);
			capture.captureEditable(pos, m_capturedEditable);
			if (!m_capturedEditable.chain)
				capture.captureSelectable(pos, m_captureSelectable);
		}

		if (m_capturedEditable.chain)
		{
// 			Vector screen = m_editPanel->transPosProjectToScreen(m_capturedEditable.pos);
// 			bNotDeliver = TBase::onMouseLeftDown(screen.x, screen.y);

			m_polyline.push_back(m_capturedEditable.pos);
		}
		else if (m_captureSelectable.chain)
		{
			m_polyline.push_back(m_captureSelectable.pos);
		}
		else
		{
			if (m_polyline.empty())
				m_lastLeftDownPos = Vector(x, y);

			if (tolerance != 0 && m_polyline.empty())
			{
				Vector pos = m_editPanel->transPosScreenToProject(x, y);
				InterruptChainVisitor interrupt(pos, tolerance);
				m_shapesImpl->traverseShapes(interrupt, e_editable);
				if (interrupt.getInterruptedChain())
				{
					m_capturedEditable.chain = interrupt.getInterruptedChain();
					m_capturedEditable.pos = pos;
					m_editPanel->Refresh();
				}
				else
					bNotDeliver = TBase::onMouseLeftDown(x, y);
			}
			else
				bNotDeliver = TBase::onMouseLeftDown(x, y);
		}
	}

	return bNotDeliver;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseLeftUp(int x, int y) 
{
	if (TBase::onMouseLeftUp(x, y)) return true;

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		NearestNodeVisitor nearest(pos, tolerance);
		m_shapesImpl->traverseShapes(nearest, e_visible);
		float dis = Math::getDistance(nearest.getNearestNode(), pos);
		if (dis < tolerance)
		{
			if (m_capturedEditable.chain)
			{
				m_capturedEditable.chain->changeVertices(m_capturedEditable.pos, nearest.getNearestNode());
				m_capturedEditable.chain->refresh();
				m_capturedEditable.pos = nearest.getNearestNode();
				m_editPanel->Refresh();
			}
		}
	}

	if (m_bSelectOpen)
	{
		m_selectOP->onMouseLeftUp(x, y);
		m_bSelectOpen = false;
		m_lastLeftDownPos.setInvalid();
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseRightDown(int x, int y) 
{
	if (m_polyline.empty())
	{
		int tolerance;
		if (m_cmpt)
			tolerance = m_cmpt->getNodeCaptureDistance();
		else
			tolerance = 0;
		if (tolerance != 0)
		{
			NodeCapture capture(m_shapesImpl, tolerance);
			capture.captureEditable(m_editPanel->transPosScreenToProject(x, y), m_capturedEditable);
			if (m_capturedEditable.chain)
			{
				m_capturedEditable.chain->removeVertices(m_capturedEditable.pos);
				m_capturedEditable.chain = NULL;
				m_editPanel->Refresh();
			}
		}

		return false;
	}
	else
	{
		return TBase::onMouseRightDown(x, y);
	}
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseMove(int x, int y) 
{
	if (TBase::onMouseMove(x, y)) return true;

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		NodeCapture capture(m_shapesImpl, tolerance);
		{
			ChainShape* oldChain = m_capturedEditable.chain;
			capture.captureEditable(pos, m_capturedEditable);
			if (oldChain && !m_capturedEditable.chain || !oldChain && m_capturedEditable.chain)
				m_editPanel->Refresh();
		}
		{
			ChainShape* oldChain = m_captureSelectable.chain;
			capture.captureSelectable(pos, m_captureSelectable);
			if (oldChain && !m_captureSelectable.chain || !oldChain && m_captureSelectable.chain)
				m_editPanel->Refresh();
		}
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseDrag(int x, int y) 
{
	if (TBase::onMouseDrag(x, y)) return true;

	if (m_bSelectOpen)
		m_selectOP->onMouseDrag(x, y);
	else if (m_capturedEditable.chain)
	{
		if (m_polyline.size() > 1)
		{
			m_capturedEditable.chain = NULL;
			return false;
		}

		if (m_polyline.size() == 1)
			m_polyline.clear();

		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_capturedEditable.chain->changeVertices(m_capturedEditable.pos, pos);
		m_capturedEditable.chain->refresh();
		m_capturedEditable.pos = pos;
		m_editPanel->Refresh();
	}
	else if (m_lastLeftDownPos.isValid() 
		&& Math::getDistance(m_lastLeftDownPos, Vector(x, y)) < DRAG_SELECT_TOL)
		//		&& (m_lastLeftDownPos.x != x || m_lastLeftDownPos.y != y))
	{
		if (m_polyline.size() == 1)
			m_polyline.clear();
		m_bSelectOpen = true;
		m_selectOP->onMouseLeftDown(x, y);
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onDraw() const 
{
	if (TBase::onDraw()) return true;

	m_selectOP->onDraw();
	if (m_cmpt)
	{
		if (m_capturedEditable.chain)
			PrimitiveDraw::drawCircle(m_capturedEditable.pos, m_cmpt->getNodeCaptureDistance(), Colorf(1.0f, 0.4f, 0.4f));
		else if (m_captureSelectable.chain)
			PrimitiveDraw::drawCircle(m_captureSelectable.pos, m_cmpt->getNodeCaptureDistance(), Colorf(1.0f, 0.4f, 0.4f));
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
clear() 
{
	if (TBase::clear()) return true;

	m_selectOP->clear();
	m_capturedEditable.chain = NULL;
	m_captureSelectable.chain = NULL;

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineOP::InterruptChainVisitor
//////////////////////////////////////////////////////////////////////////

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::InterruptChainVisitor::
InterruptChainVisitor(const Vector& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
	, m_chain(NULL)
{
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::InterruptChainVisitor::
visit(ICloneable* object, bool& bFetchNext) 
{
	Rect rect(m_pos, m_tol, m_tol);

	ChainShape* chain = static_cast<ChainShape*>(object);
	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	size_t iPos;
	const std::vector<Vector>& vertices = chain->getVertices();
	float dis = Math::getDisPointToPolyline(m_pos, vertices, &iPos);
	if (dis < m_tol)
	{
		chain->insertVertices(iPos + 1, m_pos);
		m_chain = chain;
		bFetchNext = false;
		return;
	}
	else if (chain->isClosed() && vertices.size() > 1)
	{
		float dis = Math::getDisPointToSegment(m_pos, vertices.front(), vertices.back());
		if (dis < m_tol)
		{
			chain->insertVertices(vertices.size(), m_pos);
			m_chain = chain;
			bFetchNext = false;
			return;
		}
	}

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineOP::NearestNodeVisitor
//////////////////////////////////////////////////////////////////////////

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::NearestNodeVisitor::
NearestNodeVisitor(const Vector& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
{
	m_dis = FLT_MAX;
	m_nearest.setInvalid();
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::NearestNodeVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	Rect rect(m_pos, m_tol, m_tol);

	ChainShape* chain = static_cast<ChainShape*>(object);
	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	const std::vector<Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		float dis = Math::getDistance(m_pos, vertices[i]);
		if (dis < m_dis && dis != 0)
		{
			m_dis = dis;
			m_nearest = vertices[i];
		}
	}

	bFetchNext = true;
}


#endif // D2D_EDIT_POLYLINE_OP_CPP
