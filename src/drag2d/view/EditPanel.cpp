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

#include "EditPanel.h"

#include "operator/AbstractEditOP.h"
#include "history/AbstractAtomicOP.h"
#include "view/Camera.h"
#include "view/GLCanvas.h"

using namespace d2d;

BEGIN_EVENT_TABLE(EditPanel, wxPanel)
	EVT_MOUSE_EVENTS(EditPanel::onMouse)
	EVT_KEY_DOWN(EditPanel::onKeyDown)
	EVT_KEY_UP(EditPanel::onKeyUp)
	EVT_SIZE(EditPanel::onSize)

	EVT_MENU(Menu_UpOneLayer, EditPanel::onMenuUpOneLayer)
	EVT_MENU(Menu_DownOneLayer, EditPanel::onMenuDownOneLayer)
END_EVENT_TABLE()

std::string EditPanel::menu_entries[] = 
{
	"Up One Layer",
	"Down One Layer"
};

EditPanel::EditPanel(wxWindow* parent)
	: wxPanel(parent)
{
	m_editOP = NULL;
	m_canvas = NULL;
	m_camera = new Camera;
	m_camera->addObserver(this);
	SetFocus();		// For onMouseWheelRotation
}

EditPanel::~EditPanel()
{
	delete m_camera;
	delete m_canvas;
	if (m_editOP) m_editOP->release();
	clearAtomicOPStack(m_undoStack);
	clearAtomicOPStack(m_redoStack);
}

void EditPanel::onCameraChanged()
{
	if (m_canvas)
		m_canvas->resetViewport();
}

void EditPanel::clear()
{
	m_editOP->clear();
}

Vector EditPanel::transPosScreenToProject(int x, int y) const
{
	return m_camera->transPosScreenToProject(x, y, GetSize().GetWidth(), GetSize().GetHeight());
}

// Vector EditPanel::transPosProjectToScreen(const Vector& proj) const
// {
// 	return m_camera->transPosProjectToScreen(proj, GetSize().GetWidth(), GetSize().GetHeight());
// }

void EditPanel::drawEditTemp() const
{
	if (m_editOP) m_editOP->onDraw();
}

void EditPanel::setEditOP(AbstractEditOP* editOP)
{
	if (m_editOP)
	{
		m_editOP->clear();
		m_editOP->release();
	}

	editOP->onActive();
	editOP->retain();
	m_editOP = editOP;
}

void EditPanel::onMouse(wxMouseEvent& event)
{
	if (!m_editOP) return;

	if (event.LeftDown())
		m_editOP->onMouseLeftDown(event.GetX(), event.GetY());
	else if (event.LeftUp())
		m_editOP->onMouseLeftUp(event.GetX(), event.GetY());
	else if (event.RightDown())
		m_editOP->onMouseRightDown(event.GetX(), event.GetY());
	else if (event.RightUp())
		m_editOP->onMouseRightUp(event.GetX(), event.GetY());
	else if (event.Moving())
		m_editOP->onMouseMove(event.GetX(), event.GetY());
	else if (event.Dragging())
		m_editOP->onMouseDrag(event.GetX(), event.GetY());
	else if (event.LeftDClick())
		m_editOP->onMouseLeftDClick(event.GetX(), event.GetY());
	else if (event.GetWheelRotation())
		m_editOP->onMouseWheelRotation(event.GetX(), event.GetY(), event.GetWheelRotation());
}

void EditPanel::onKeyDown(wxKeyEvent& event)
{
	if (wxGetKeyState(WXK_CONTROL_Z))
		undo();
	else if (wxGetKeyState(WXK_CONTROL_Y))
		redo();
	
	m_editOP->onKeyDown(event.GetKeyCode());
}

void EditPanel::onKeyUp(wxKeyEvent& event)
{
	m_editOP->onKeyUp(event.GetKeyCode());
}

void EditPanel::onMouseWheelRotation(int x, int y, int direction)
{
	const float oldScale = m_camera->getScale();
	const float newScale = direction > 0 ? oldScale / 1.1f : oldScale * 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(GetSize().GetHeight() - y);
	m_camera->setScale(newScale, cx, cy, GetSize().GetWidth(), GetSize().GetHeight());
	Refresh();
}

void EditPanel::resetCanvas()
{
	if (m_canvas)
	{
		m_canvas->resetInitState();
		Refresh();
	}
}

void EditPanel::undo()
{
	if (!m_undoStack.empty())
	{
		AbstractAtomicOP* op = m_undoStack.top();
		m_undoStack.pop();
		op->undo();
		m_redoStack.push(op);
		Refresh();
	}
}

void EditPanel::redo()
{
	if (!m_redoStack.empty())
	{
		AbstractAtomicOP* op = m_redoStack.top();
		m_redoStack.pop();
		op->redo();
		m_undoStack.push(op);
		Refresh();
	}
}

void EditPanel::addHistoryOP(AbstractAtomicOP* op)
{
	m_undoStack.push(op);
	clearAtomicOPStack(m_redoStack);
}

void EditPanel::onMenuUpOneLayer(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_UpOneLayer);
	Refresh();
}

void EditPanel::onMenuDownOneLayer(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_DownOneLayer);
	Refresh();
}

void EditPanel::onSize(wxSizeEvent& event)
{
	if (m_canvas)
		m_canvas->SetSize(event.GetSize());
	m_camera->setCenter(Vector(0.0f, 0.0f));
	Refresh();	// no refresh when change window size
}

void EditPanel::clearAtomicOPStack(std::stack<AbstractAtomicOP*>& stack)
{
	while (!stack.empty())
	{
		delete stack.top();
		stack.pop();
	}
}
