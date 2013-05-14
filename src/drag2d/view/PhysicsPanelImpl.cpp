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

#include "PhysicsPanelImpl.h"

#include "common/Math.h"
#include "dataset/BodyFactory.h"
#include "render/GL10.h"

using namespace d2d;

PhysicsPanelImpl::PhysicsPanelImpl()
	: m_debugDraw(BOX2D_SCALE_FACTOR)
{
	m_world = createWorld(Vector(0.0f, -10.0f));

	BodyFactory::setWorld(m_world);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	m_debugDraw.SetFlags(flags);

	m_timer = new wxTimer(this, 1);
	m_timer->Start(1.0f / 60);
	m_isPaused = false;

	Connect(wxEVT_TIMER, wxCommandEventHandler(PhysicsPanelImpl::update));
}

PhysicsPanelImpl::~PhysicsPanelImpl()
{
	pause();
}

void PhysicsPanelImpl::update(wxCommandEvent& event)
{
	const float timeStep = 1.0f / 60;
	m_world->Step(timeStep, 8, 3);
}

void PhysicsPanelImpl::drawPhysics()
{
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);

	GL10::Enable(GL10::GL_BLEND);
	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);
	m_world->DrawDebugData();
	GL10::Disable(GL10::GL_BLEND);

	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);
}

b2World* PhysicsPanelImpl::createWorld(const Vector& gravity)
{
	b2World* world;

	b2Vec2 b2gravity;
	b2gravity.Set(gravity.x, gravity.y);
	world = new b2World(b2gravity);

	world->SetContactListener(this);
	world->SetDebugDraw(&m_debugDraw);

	return world;
}

void PhysicsPanelImpl::pause()
{
	m_isPaused = !m_isPaused;
	if (m_isPaused)
		m_timer->Stop();
	else
		m_timer->Start(1.0f / 60);
}
