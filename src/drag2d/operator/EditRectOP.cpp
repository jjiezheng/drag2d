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

#include "EditRectOP.h"

#include <vector>

#include "common/Math.h"
#include "component/NodeCaptureCMPT.h"
#include "dataset/ChainShape.h"
#include "view/MultiShapesImpl.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

EditRectOP::EditRectOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
					   NodeCaptureCMPT<EditRectOP>* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
	clear();
}

bool EditRectOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(pos, m_captured);
	}

	if (m_captured.chain && isRect(m_captured.chain))
		;
	else
		m_start = pos;

	return false;
}

bool EditRectOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (m_start.isValid())
	{
		m_end = m_editPanel->transPosScreenToProject(x, y);
		if (Math::getDistance(m_start, m_end) > 1)
		{
			std::vector<Vector> rect(4);
			rect[0] = m_start;
			rect[1].set(m_start.x, m_end.y);
			rect[2] = m_end;
			rect[3].set(m_end.x, m_start.y);
			m_shapesImpl->insertShape(new ChainShape(rect, true));
		}
		clear();
	}

	return false;
}

bool EditRectOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		ChainShape* old = m_captured.chain;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.chain || !old && m_captured.chain)
			m_editPanel->Refresh();
	}

	return false;
}

bool EditRectOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_start.isValid())
	{
		m_end = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}
	else if (m_captured.chain)
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);

		std::vector<Vector> rect = m_captured.chain->getVertices();
		assert(rect.size() == 4);
		size_t i;
		for (i = 0; i < 4 && rect[i] != m_captured.pos; ++i)
			;
		size_t last = (i - 1 + 4) % 4,
			next = (i + 1 + 4) % 4;
		if (rect[last].x == rect[i].x)
		{
			rect[last].x = pos.x;
			rect[next].y = pos.y;
		}
		else
		{
			rect[next].x = pos.x;
			rect[last].y = pos.y;
		}
		rect[i] = pos;
		m_captured.chain->setVertices(rect);

		m_captured.chain->refresh();
		m_captured.pos = pos;
		m_editPanel->Refresh();
	}

	return false;
}

bool EditRectOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;
	
	if (m_start.isValid() && m_end.isValid())
		PrimitiveDraw::drawSquareFrame(m_start, m_end);
	if (m_cmpt && m_captured.chain)
		PrimitiveDraw::drawCircle(m_captured.pos, m_cmpt->getNodeCaptureDistance(), Colorf(1.0f, 0.4f, 0.4f));

	return false;
}

bool EditRectOP::clear()
{
	if (ZoomViewOP::clear()) return true;
	m_start.setInvalid();
	m_end.setInvalid();
	return false;
}

bool EditRectOP::isRect(const ChainShape* chain)
{
	const std::vector<Vector>& vertices = chain->getVertices();
	if (vertices.size() == 4)
	{
		if (vertices[1].x == vertices[0].x && vertices[1].y == vertices[2].y && 
			vertices[3].x == vertices[2].x && vertices[3].y == vertices[0].y)
			return true;

		if (vertices[1].x == vertices[2].x && vertices[1].y == vertices[0].y && 
			vertices[3].x == vertices[0].x && vertices[3].y == vertices[2].y)
			return true;
	}
	return false;
}