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

#include "DragPhysicsOP.h"

#include "common/PhysicsQuery.h"
#include "common/Math.h"

using namespace d2d;

DragPhysicsOP::DragPhysicsOP(EditPanel* editPanel, b2World* world, b2Body* ground)
	: ZoomViewOP(editPanel, true)
	, m_world(world)
	, m_ground(ground)
	, m_mouseJoint(NULL)
{
	currPos.setInvalid();
}

bool DragPhysicsOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	if (m_mouseJoint) return false;

	const Vector pos = m_editPanel->transPosScreenToProject(x, y);
	b2Body* body = PhysicsQuery::queryOn(m_world, pos);
	if (body && body != m_ground)
	{
		b2MouseJointDef md;
		md.bodyA = m_ground;
		md.bodyB = body;
		md.target = b2Vec2(pos.x / BOX2D_SCALE_FACTOR, pos.y / BOX2D_SCALE_FACTOR);
		md.maxForce = 1000.0f * body->GetMass();
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		body->SetAwake(true);
	}

	return false;
}

bool DragPhysicsOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
		currPos.setInvalid();
	}

	return false;
}

bool DragPhysicsOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_mouseJoint)
	{
		currPos = m_editPanel->transPosScreenToProject(x, y);
		m_mouseJoint->SetTarget(b2Vec2(currPos.x / BOX2D_SCALE_FACTOR, currPos.y / BOX2D_SCALE_FACTOR));
	}

	return false;
}
