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

#include "DrawCurveOP.h"

#include <gl/glu.h>

using namespace d2d;

DrawCurveOP::DrawCurveOP(EditPanel* editPanel)
	: ZoomViewOP(editPanel, true)
	, m_startDraw(false)
{
	m_firstPos.setInvalid();
}

bool DrawCurveOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos.set(x, y);

	return false;
}

bool DrawCurveOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (!m_startDraw && m_firstPos.isValid() && 
		(m_firstPos.x != x || m_firstPos.y != y))
		m_startDraw = true;

	if (m_startDraw)
	{
		m_curve.push_back(m_editPanel->transPosScreenToProject(x, y));
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawCurveOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_curve.empty()) return false;

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0, n = m_curve.size(); i < n; ++i)
		glVertex2f(m_curve[i].x, m_curve[i].y);
	glEnd();

	return false;
}

bool DrawCurveOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_curve.clear();
	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}
