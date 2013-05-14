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

#include "PhysicsQuery.h"

#include "common/Vector.h"
#include "common/Math.h"

using namespace d2d;

b2Body* PhysicsQuery::queryOn(b2World* world, const Vector& pos)
{
	const b2Vec2 b2Pos(pos.x / BOX2D_SCALE_FACTOR, pos.y / BOX2D_SCALE_FACTOR);

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = b2Pos - d;
	aabb.upperBound = b2Pos + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(b2Pos);
	world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
		return callback.m_fixture->GetBody();
	else
		return NULL;
}

b2Body* PhysicsQuery::queryNear(b2World* world, const Vector& pos)
{
	b2Body* onBody = queryOn(world, pos);
	if (onBody)
	{
		return onBody;
	}
	else
	{
		b2Body* nearBody = NULL;
		float nearDis = FLT_MAX;
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			b2Vec2 bPos = b->GetPosition();
			const float dis = Math::getDistance(pos, Vector(bPos.x, bPos.y));
			if (dis < nearDis)
			{
				nearDis = dis;
				nearBody = b;
			}
		}
		return nearBody;
	}
}

//////////////////////////////////////////////////////////////////////////
// class PhysicsQuery::QueryCallback
//////////////////////////////////////////////////////////////////////////

PhysicsQuery::QueryCallback::QueryCallback(const b2Vec2& point)
{
	m_point = point;
	m_fixture = NULL;
}

bool PhysicsQuery::QueryCallback::ReportFixture(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();
	if (body->GetType() == b2_dynamicBody)
	{
		bool inside = fixture->TestPoint(m_point);
		if (inside)
		{
			m_fixture = fixture;

			// We are done, terminate the query.
			return false;
		}
	}

	// Continue the query.
	return true;
}
