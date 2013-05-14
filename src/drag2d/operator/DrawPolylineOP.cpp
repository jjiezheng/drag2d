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

#include "DrawPolylineOP.h"

#include "common/Math.h"
#include "view/Camera.h"

#include <gl/glu.h>

using namespace d2d;

DrawPolylineOP::DrawPolylineOP(EditPanel* editPanel, bool isClosed)
	: ZoomViewOP(editPanel, true)
{
	m_isClosed = isClosed;
	m_currPos.setInvalid();
}

bool DrawPolylineOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_polyline.push_back(pos);
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseRightDown(int x, int y)
{
	if (ZoomViewOP::onMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) m_currPos.setInvalid();
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawPolylineOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_currPos = pos;
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseLeftDClick(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_polyline.empty()) return false;

	glColor3f(0, 0, 0);

	glLineWidth(2.0f);
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0, n = m_polyline.size(); i < n; ++i)
		glVertex2f(m_polyline[i].x, m_polyline[i].y);
	if (m_currPos.isValid())
		glVertex2f(m_currPos.x, m_currPos.y);
	glEnd();
	glPopMatrix();
	glLineWidth(1.0f);

	return false;
}

bool DrawPolylineOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

bool DrawPolylineOP::shouldFixMousePos() const
{
	return !m_polyline.empty() && wxGetKeyState(WXK_SHIFT);
}

void DrawPolylineOP::fixPosTo45Degree(Vector& pos) const
{
	Vector last = m_polyline.back();

	float nearest;
	Vector fixed = pos;

	const float dx = fabs(pos.x - last.x);
	nearest = dx;
	fixed.set(last.x, pos.y);

	const float dy = fabs(pos.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.set(pos.x, last.y);
	}

	Vector other(last.x + 1, last.y - 1);
	const float dxyDown = Math::getDisPointToStraightLine(pos, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	other.set(last.x + 1, last.y + 1);
	const float dxyUp = Math::getDisPointToStraightLine(pos, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	pos = fixed;
}
