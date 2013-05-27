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

#include "SelectShapesOP.h"

#include "common/visitors.h"
#include "dataset/PolygonShape.h"
#include "dataset/CircleShape.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/CirclePropertySetting.h"
#include "view/MultiShapesImpl.h"
#include "render/DrawSelectedShapeVisitor.h"

using namespace d2d;

SelectShapesOP::SelectShapesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							   PropertySettingPanel* propertyPanel, 
							   AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_shapeImpl(shapesImpl)
	, m_propertyPanel(propertyPanel)
	, m_lastCtrlPress(false)
{
	m_selection = shapesImpl->getShapeSelection();
	m_selection->retain();

	m_firstPos.setInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	clearClipboard();

 	m_selection->clear();
 	m_selection->release();
}

bool SelectShapesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_shapeImpl->removeShapeSelection();
	else if (wxGetKeyState(WXK_CONTROL_X))
	{
		clearClipboard();
		m_selection->traverse(FetchAllVisitor<IShape>(m_clipboard));
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
			m_clipboard[i]->retain();
		m_shapeImpl->removeShapeSelection();
	}
	else if (m_lastCtrlPress && (keyCode == 'c' || keyCode == 'C')/*wxGetKeyState(WXK_CONTROL_C)*/)
	{
		clearClipboard();

		std::vector<PolygonShape*> polys;
		m_selection->traverse(FetchAllVisitor<PolygonShape>(polys));
		for (size_t i = 0, n = polys.size(); i < n; ++i)
			m_clipboard.push_back(polys[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL_V))
	{
		// todo: should deep copy
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			m_clipboard[i]->retain();
			m_shapeImpl->insertShape(m_clipboard[i]);
			m_editPanel->Refresh();
		}
	}

	m_lastCtrlPress = keyCode == WXK_CONTROL;

	return false;
}

bool SelectShapesOP::onMouseLeftDown(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	IShape* selected = m_shapeImpl->queryShapeByPos(pos);
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->isExist(selected))
				m_selection->erase(selected);
			else
			{
				m_selection->insert(selected);
				if (m_selection->size() == 1)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
				else
					m_propertyPanel->setPropertySetting(NULL);
			}
		}
		else
		{
			if (!m_selection->isExist(selected))
			{
				m_selection->clear();
				m_selection->insert(selected);
				if (m_propertyPanel)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		m_selection->clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectShapesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		Rect rect(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		std::vector<IShape*> shapes;
		m_shapeImpl->queryShapesByRect(rect, shapes);
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_selection->insert(shapes[i]);

		if (m_propertyPanel)
		{
			if (m_selection->size() == 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(shapes[0]));
			else
				m_propertyPanel->setPropertySetting(NULL);
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

	return false;
}

bool SelectShapesOP::onDraw() const
{
	if (DrawRectangleOP::onDraw()) return true;

	m_selection->traverse(DrawSelectedShapeVisitor());

	return false;
}

bool SelectShapesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	clearClipboard();
	m_selection->clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectShapesOP::createPropertySetting(IShape* shape) const
{
	if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		return new CirclePropertySetting(m_editPanel, circle);
	else
		return NULL;
}

void SelectShapesOP::clearClipboard()
{
 	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
 		m_clipboard[i]->release();
 	m_clipboard.clear();
}
