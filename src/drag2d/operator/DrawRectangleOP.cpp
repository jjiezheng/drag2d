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

#include "DrawRectangleOP.h"

#include <gl/glu.h>

using namespace d2d;

DrawRectangleOP::DrawRectangleOP(EditPanel* editPanel,
								 const Colorf& color /*= Colorf(0, 0, 0)*/)
	: ZoomViewOP(editPanel, true)
	, m_color(color)
{
	m_firstPos.setInvalid();
	m_currPos.setInvalid();
}

bool DrawRectangleOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_firstPos.isValid())
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawRectangleOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_firstPos.isValid() && m_currPos.isValid())
	{
		glColor3f(m_color.r, m_color.g, m_color.b);
		glLineWidth(2.0f);
		glPushMatrix();
		glBegin(GL_LINE_LOOP);
			glVertex3f(m_firstPos.x, m_firstPos.y, -1.0f);
			glVertex3f(m_currPos.x, m_firstPos.y, -1.0f);
			glVertex3f(m_currPos.x, m_currPos.y, -1.0f);
			glVertex3f(m_firstPos.x, m_currPos.y, -1.0f);
		glEnd();
		glPopMatrix();
		glLineWidth(1.0f);
	}

	return false;
}

bool DrawRectangleOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();

	return false;
}
