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

#ifndef D2D_GL_CANVAS_H
#define D2D_GL_CANVAS_H

#include <wx/glcanvas.h>

#include "common/Color.h"

namespace d2d
{
	class Camera;
	class EditPanel;

	class GLCanvas : public wxGLCanvas
	{
	public:
		GLCanvas(EditPanel* editPanel);
		virtual ~GLCanvas();

		virtual void clear() {}

		void resetInitState();		// Another GLCanvas closed, refresh the under one

		void resetViewport();		// On Mouse Wheel
									// onSize no use, if the size not change
									// also can put gluOrtho2D in each onPaint, save this and Camera's observer pattern

		void setBgColor(const Colorf& color);

	protected:
		virtual void initGL();
		virtual void onSize(int w, int h) = 0;
		virtual void onDraw() = 0;

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
 		void onMouse(wxMouseEvent& event);
 		void onKeyDown(wxKeyEvent& event);
		void onKeyUp(wxKeyEvent& event);

	protected:
		EditPanel* m_editPanel;

		Camera* m_camera;

		Colorf m_bgColor;

		int m_width, m_height;

	private:
		bool m_isInit;
		wxGLContext* m_context;

		DECLARE_EVENT_TABLE()

	}; // GLCanvas
}

#endif // D2D_GL_CANVAS_H
