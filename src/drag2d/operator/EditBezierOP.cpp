#include "EditBezierOP.h"

#include "common/Math.h"
#include "dataset/BezierShape.h"
#include "view/MultiShapesImpl.h"
#include "view/PropertySettingPanel.h"
#include "view/BezierPropertySetting.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

EditBezierOP::EditBezierOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
						   PropertySettingPanel* propertyPanel, 
						   NodeCaptureCMPT<EditBezierOP>* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
	clear();
}

bool EditBezierOP::onKeyDown(int keyCode)
{
	if (ZoomViewOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapesImpl->removeShapeSelection();
		m_captured.clear();
		m_editPanel->Refresh();

		m_propertyPanel->setPropertySetting(NULL);
	}

	return false;
}

bool EditBezierOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_editPanel->transPosScreenToProject(x, y);

	m_shapesImpl->getShapeSelection()->clear();

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

 		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
 			m_propertyPanel->setPropertySetting(new BezierPropertySetting(m_editPanel, bezier));
			m_shapesImpl->getShapeSelection()->insert(bezier);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditBezierOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.isValid())
		{
			m_currPos = m_editPanel->transPosScreenToProject(x, y);

			const float dis = Math::getDistance(m_firstPress, m_currPos);
			if (dis > 1)
			{
				BezierShape* bezier = new BezierShape(m_firstPress, m_currPos);
				m_propertyPanel->setPropertySetting(new BezierPropertySetting(m_editPanel, bezier));
				m_shapesImpl->getShapeSelection()->insert(bezier);
				m_shapesImpl->insertShape(bezier);
			}
		}
	}
	else
	{
 		m_propertyPanel->enablePropertyGrid(true);
 		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
 			m_propertyPanel->setPropertySetting(new BezierPropertySetting(m_editPanel, bezier));
	}

	clear();

	m_editPanel->Refresh();

	return false;
}

bool EditBezierOP::onMouseRightDown(int x, int y)
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
			m_shapesImpl->getShapeSelection()->clear();
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

bool EditBezierOP::onMouseMove(int x, int y)
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

bool EditBezierOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);

	if (m_captured.shape)
	{
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			Vector center(bezier->getRect().xCenter(), bezier->getRect().yCenter());

			// move
			if (!m_captured.pos.isValid())
			{
				Vector offset = m_currPos - center;
				for (size_t i = 0; i < 4; ++i)
					bezier->points[i] += offset;
				bezier->createCurve();
			}
			// change control point
			else
			{
				int i = 0;
				for ( ; i < 4 && bezier->points[i] != m_captured.pos; ++i)
					;
				assert(i != 4);
				bezier->points[i] = m_currPos;
				bezier->createCurve();

				m_captured.pos = m_currPos;
			}

			m_propertyPanel->enablePropertyGrid(false);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditBezierOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_captured.shape)
	{
		if (m_cmpt)
		{
			if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
			{
				PrimitiveDraw::drawCircle(Vector(bezier->getRect().xCenter(), bezier->getRect().yCenter()), 
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
		{
			BezierShape bezier(m_firstPress, m_currPos);
			bezier.draw();
		}
//			PrimitiveDraw::drawRect(m_firstPress, m_currPos);
	}

	return false;
}

bool EditBezierOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}
} // d2d