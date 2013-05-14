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

#include "UniversalArrangeOP.h"
#include "DragPhysicsOP.h"

#include "common/PhysicsQuery.h"

using namespace d2d;

UniversalArrangeOP::UniversalArrangeOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback/* = NULL*/)
	: ZoomViewOP(editPanel, true)
{
	m_noPhysics = new ArrangeSpriteOP<SelectSpritesOP>(editPanel, spritesImpl, propertyPanel, callback);
	m_editOP = m_noPhysics;
}

UniversalArrangeOP::~UniversalArrangeOP()
{
	m_noPhysics->release();
	m_noPhysics = NULL;

	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
		m_physics[i].editOP->release();
	m_physics.clear();

	m_editOP = NULL;
}

bool UniversalArrangeOP::onKeyDown(int keyCode)
{
	return m_editOP->onKeyDown(keyCode);	
}

bool UniversalArrangeOP::onMouseLeftDown(int x, int y)
{
	m_editOP = m_noPhysics;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
	{
		if (PhysicsQuery::queryOn(m_physics[i].world, pos))
		{
			m_editOP = m_physics[i].editOP;
			break;
		}
	}

	return m_editOP->onMouseLeftDown(x, y);
}

bool UniversalArrangeOP::onMouseLeftUp(int x, int y)
{
	return m_editOP->onMouseLeftUp(x, y);
}

bool UniversalArrangeOP::onMouseRightDown(int x, int y)
{
	return m_editOP->onMouseRightDown(x, y);
}

bool UniversalArrangeOP::onMouseRightUp(int x, int y)
{
	return m_editOP->onMouseRightUp(x, y);
}

bool UniversalArrangeOP::onMouseDrag(int x, int y)
{
	return m_editOP->onMouseDrag(x, y);
}

bool UniversalArrangeOP::onDraw() const
{
	return m_editOP->onDraw();
}

bool UniversalArrangeOP::clear()
{
	return m_editOP->clear();
}

void UniversalArrangeOP::addPhysicsEditOP(b2World* world, b2Body* ground)
{
	PhysicsOP physics;
	physics.world = world;
	physics.editOP = new DragPhysicsOP(m_editPanel, world, ground);
	m_physics.push_back(physics);
}
