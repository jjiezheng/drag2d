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

#ifndef D2D_PHYSICS_PANEL_IMPL_H
#define D2D_PHYSICS_PANEL_IMPL_H

#include "render/b2Render.h"

#include <Box2D/Box2D.h>
#include <wx/wx.h>

namespace d2d
{
	class Vector;

	class PhysicsPanelImpl : public b2ContactListener, public wxEvtHandler
	{
	public:
		PhysicsPanelImpl();
		virtual ~PhysicsPanelImpl();

		virtual void update(wxCommandEvent& event);

		virtual void drawPhysics();

		b2World* createWorld(const Vector& gravity);

		void pause();

		b2World* getWorld() { return m_world; }

	protected:
		b2World* m_world;

	private:
		b2GLRender m_debugDraw;

		wxTimer* m_timer;
		bool m_isPaused;

	}; // PhysicsPanelImpl
}

#endif // D2D_PHYSICS_PANEL_IMPL_H
