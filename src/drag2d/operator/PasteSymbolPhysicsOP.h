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

#ifndef D2D_PASTE_SYMBOL_PHYSICS_OP_H
#define D2D_PASTE_SYMBOL_PHYSICS_OP_H

#include "PasteSymbolOP.h"

#include <Box2D/Box2D.h>

namespace d2d
{
	class PhysicsPanelImpl;

	class PasteSymbolPhysicsOP : public PasteSymbolOP
	{
	public:
		PasteSymbolPhysicsOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, float* pScale = NULL);

		virtual bool onMouseLeftDown(int x, int y);

		void setStatic(bool bStatic) { m_bStatic = bStatic; }

	protected:
		bool m_bStatic;

	private:
		b2World* m_world;

		float* m_pScale;

	}; // PasteSymbolPhysicsOP
}

#endif // D2D_PASTE_SYMBOL_PHYSICS_OP_H
