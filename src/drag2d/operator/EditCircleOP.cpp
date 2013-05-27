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

#include "EditCircleOP.h"

#include "common/Math.h"
#include "view/MultiShapesImpl.h"
#include "dataset/CircleShape.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

EditCircleOP::EditCircleOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
						   NodeCaptureCMPT<EditCircleOP>* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
	m_firstPress.setInvalid();
	m_currPos.setInvalid();
}

bool EditCircleOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_editPanel->transPosScreenToProject(x, y);

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);

		const float radius = Math::getDistance(m_firstPress, m_currPos);
		if (radius > 0)
			m_shapesImpl->insertShape(new CircleShape(m_firstPress, radius));
	}

	clear();

	m_editPanel->Refresh();

	return false;
}

bool EditCircleOP::onMouseRightDown(int x, int y)
{
	if (ZoomViewOP::onMouseRightDown(x, y)) return true;

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);

		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_currPos, m_captured);
		if (m_captured.shape)
		{
			m_shapesImpl->removeShape(m_captured.shape);
			m_captured.clear();
			m_editPanel->Refresh();
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::onMouseMove(int x, int y)
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
		IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape)
			m_editPanel->Refresh();
	}

	return false;
}

bool EditCircleOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);

	if (m_captured.shape)
	{
		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			// move  
			if (m_captured.pos.isValid())
				circle->center = m_currPos;
			// change size
			else
				circle->radius = Math::getDistance(m_currPos, circle->center);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditCircleOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_captured.shape)
	{
		if (m_cmpt)
		{
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
				PrimitiveDraw::drawCircle(circle->center, m_cmpt->getNodeCaptureDistance(), true, 2, Colorf(1.0f, 0.4f, 0.4f));
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid())
			PrimitiveDraw::drawCircle(m_firstPress, Math::getDistance(m_firstPress, m_currPos), false, 3, Colorf(0, 0, 0), 32);
	}

	return false;
}

bool EditCircleOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}