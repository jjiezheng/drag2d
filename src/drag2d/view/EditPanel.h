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

#ifndef D2D_EDIT_PANEL_H
#define D2D_EDIT_PANEL_H

#include <wx/wx.h>
#include <stack>

#include "common/interface.h"
#include "common/Vector.h"

namespace d2d
{
	class AbstractEditOP;
	class GLCanvas;
	class Camera;
	class AbstractAtomicOP;

	class EditPanel : public wxPanel, public ICameraObserver
	{
	public:
		EditPanel(wxWindow* parent);
		virtual ~EditPanel();

 		//
 		// ICameraObserver interface
 		//
 		virtual void onCameraChanged();

		virtual void clear();

		Vector transPosScreenToProject(int x, int y) const;
//		Vector transPosProjectToScreen(const Vector& proj) const;

		void drawEditTemp() const;

		AbstractEditOP* getEditOP() const { return m_editOP; }
		void setEditOP(AbstractEditOP* editOP);

		GLCanvas* getCanvas() const { return m_canvas; }
		void setCanvas(GLCanvas* canvas) { m_canvas = canvas; }

		Camera* getCamera() const { return m_camera; }

		// In Stage, class StagePanel can't get focus, only its class GLCanvas has the focus, so 
		// these two func should be called by GLCanvas.
		// While in SymbolEdit, class SymbolEditPanel can get focus.
		void onMouse(wxMouseEvent& event);
		virtual void onKeyDown(wxKeyEvent& event);
		virtual void onKeyUp(wxKeyEvent& event);
		void onMouseWheelRotation(int x, int y, int direction);

		void resetCanvas();

		void undo();
		void redo();
		void addHistoryOP(AbstractAtomicOP* op);

	public:
		enum
		{
			Menu_UpOneLayer,
			Menu_DownOneLayer
		};

		static std::string menu_entries[];

		void onMenuUpOneLayer(wxCommandEvent& event);
		void onMenuDownOneLayer(wxCommandEvent& event);

	protected:
		void onSize(wxSizeEvent& event);

	private:
		void clearAtomicOPStack(std::stack<AbstractAtomicOP*>& stack);

	protected:
		AbstractEditOP* m_editOP;

		GLCanvas* m_canvas;
		Camera* m_camera;

		std::stack<AbstractAtomicOP*> m_undoStack, m_redoStack;

		DECLARE_EVENT_TABLE()

	}; // EditPanel
}

#endif // D2D_EDIT_PANEL_H
