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

#include "IBody.h"
#include "BodyFactory.h"

#include <Box2D/Box2D.h>

using namespace d2d;

IBody::IBody(b2World* world, float scale/* = 1.0f*/)
{
	m_alive = false;
	
	m_world = world;
	m_body = NULL;
	
	m_scale = scale;
}

IBody::~IBody()
{
	if (m_body)
	{
		m_world->DestroyBody(m_body);
		m_body = NULL;
	}
}

IBody* IBody::clone()
{
	if (m_filename == "") return NULL;

	IBody* body = BodyFactory::createBody(m_filename, m_scale);

	body->m_filename = m_filename;
	body->m_alive = m_alive;
	body->m_world = m_world;
	body->m_scale = m_scale;

	b2Body* b2body = body->getBody();
	b2body->SetTransform(m_body->GetPosition(), m_body->GetAngle());

	return body;
}

void IBody::setDensity(float density)
{
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetDensity(density);
	}
}

void IBody::setFriction(float friction)
{
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetFriction(friction);
	}
}

void IBody::setCollisionFilter(const b2Filter& filter)
{
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetFilterData(filter);
	}
}

void IBody::setTransform(const Vector& pos, float angle)
{
	m_body->SetTransform(b2Vec2(pos.x, pos.y), angle);
}
