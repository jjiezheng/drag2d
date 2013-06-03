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
#include "dataset/RectShape.h"
#include "view/MultiShapesImpl.h"
#include "view/PropertySettingPanel.h"
#include "view/RectPropertySetting.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

EditRectOP::EditRectOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
					   PropertySettingPanel* propertyPanel,
					   NodeCaptureCMPT<EditRectOP>* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
	clear();
}

bool EditRectOP::onMouseLeftDown(int x, int y)
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

		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			m_propertyPanel->setPropertySetting(new RectPropertySetting(m_editPanel, rect));
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditRectOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.isValid())
		{
			m_currPos = m_editPanel->transPosScreenToProject(x, y);

			const float radius = Math::getDistance(m_firstPress, m_currPos);
			if (radius > 1)
				m_shapesImpl->insertShape(new RectShape(m_firstPress, m_currPos));
		}
	}
	else
	{
		m_propertyPanel->enablePropertyGrid(true);
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			m_propertyPanel->setPropertySetting(new RectPropertySetting(m_editPanel, rect));
	}

	clear();

	m_editPanel->Refresh();

	return false;
}

bool EditRectOP::onMouseRightDown(int x, int y)
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

			m_propertyPanel->setPropertySetting(NULL);
		}
	}
	else
	{
		m_captured.clear();
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
		IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape)
			m_editPanel->Refresh();
	}

	return false;
}

bool EditRectOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);

	if (m_captured.shape)
	{
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			Vector center(rect->m_rect.xCenter(), rect->m_rect.yCenter());

			// move
			if (!m_captured.pos.isValid())
			{
				rect->m_rect.translate(m_currPos - center);
			}
			// change size
			else 
			{
				if (m_captured.pos.x > center.x)
					rect->m_rect.xMax = m_currPos.x;
				else
					rect->m_rect.xMin = m_currPos.x;
				if (m_captured.pos.y > center.y)
					rect->m_rect.yMax = m_currPos.y;
				else
					rect->m_rect.yMin = m_currPos.y;

				m_captured.pos = m_currPos;
			}

			m_propertyPanel->enablePropertyGrid(false);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditRectOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_captured.shape)
	{
		if (m_cmpt)
		{
			if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			{
				PrimitiveDraw::drawCircle(Vector(rect->m_rect.xCenter(), rect->m_rect.yCenter()), 
					m_cmpt->getNodeCaptureDistance(), true, 2, Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.isValid())
					PrimitiveDraw::drawCircle(m_captured.pos, m_cmpt->getNodeCaptureDistance(), 
						true, 2, Colorf(1.0f, 0.4f, 0.4f));
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid())
			PrimitiveDraw::drawRect(m_firstPress, m_currPos);
	}

	return false;
}

bool EditRectOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}