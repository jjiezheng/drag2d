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

#include "ZoomViewOP.h"

#include "view/Camera.h"
#include "common/Settings.h"

using namespace d2d;

ZoomViewOP::ZoomViewOP(EditPanel* editPanel, bool bMouseMoveFocus) 
	: AbstractEditOP(editPanel)
	, m_bMouseMoveFocus(bMouseMoveFocus)
{
	m_lastPos.setInvalid();
}

bool ZoomViewOP::onKeyDown(int keyCode)
{
	if (!Settings::bZoomOnlyUseMouseWheel && keyCode == WXK_SPACE)
		m_editPanel->SetCursor(wxCURSOR_HAND);
	return false;
}

bool ZoomViewOP::onKeyUp(int keyCode)
{
	if (!Settings::bZoomOnlyUseMouseWheel && keyCode == WXK_SPACE)
		m_editPanel->SetCursor(wxCURSOR_ARROW);
	return false;
}

bool ZoomViewOP::onMouseLeftDown(int x, int y)
{
	if (!Settings::bZoomOnlyUseMouseWheel)
		m_lastPos.setInvalid();

	if (!Settings::bZoomOnlyUseMouseWheel && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.set(x, y);
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::onMouseLeftUp(int x, int y)
{
	if (!Settings::bZoomOnlyUseMouseWheel && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.setInvalid();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::onMouseMove(int x, int y) 
{
	if (m_bMouseMoveFocus) 
		m_editPanel->SetFocus();

	return false;
}

bool ZoomViewOP::onMouseDrag(int x, int y)
{
	if (Settings::bZoomOnlyUseMouseWheel) return false;

	if (wxGetKeyState(WXK_SPACE))
	{
		if (!m_lastPos.isValid())
			m_lastPos.set(x, y);
		else
		{
			Vector currPos(x, y);

			Vector cameraPos = m_editPanel->getCamera()->getCenter();
			Vector offset = m_lastPos - currPos;
			offset *= m_editPanel->getCamera()->getScale();
			offset.y = -offset.y;
			m_editPanel->getCamera()->setCenter(cameraPos + offset);

			m_lastPos = currPos;

			m_editPanel->Refresh();
		}
		return true;
	}
	else
	{
		m_lastPos.setInvalid();
		return false;
	}
}

bool ZoomViewOP::onMouseWheelRotation(int x, int y, int direction) 
{
	if (Settings::bZoomOnlyUseMouseWheel)
		m_editPanel->onMouseWheelRotation(x, y, direction);
	else
		m_editPanel->onMouseWheelRotation(m_editPanel->GetSize().GetWidth() * 0.5f, m_editPanel->GetSize().GetHeight() * 0.5f, direction);

	return false;
}
