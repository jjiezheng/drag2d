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

#include "PolygonBody.h"
#include "BodyData.h"

#include <Box2D/Box2D.h>

#include "common/Vector.h"
#include "common/Math.h"

using namespace d2d;

PolygonBody::PolygonBody(b2World* world, const BodyData& bodyData, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	m_filename = bodyData.getFilename();

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	const std::vector<BodyData::FixtureData*>& fixturesData = bodyData.getAllFixturesData();
	for (size_t i = 0, n = fixturesData.size(); i < n; ++i)
	{
		const BodyData::FixtureData* fixtureData = fixturesData[i];

		const size_t size = fixtureData->vertices.size();
		std::vector<b2Vec2> transList(size);
		for (size_t j = 0, m = size; j < m; ++j)
		{
			transList[j].x = fixtureData->vertices[j].x * scale / BOX2D_SCALE_FACTOR;
			transList[j].y = fixtureData->vertices[j].y * scale / BOX2D_SCALE_FACTOR;
		}

		b2PolygonShape shape;
		shape.Set(&transList[0], size);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.friction = 0.5f;
		fixtureData->setFixtureDef(fd);

		m_body->CreateFixture(&fd);
	}
}

PolygonBody::PolygonBody(b2World* world, float hWidth, float hHeight, 
						 bool isStatic, float scale/* = 1.0f*/)
	: IBody(world, scale)
{	
	b2BodyDef bd;
	bd.type = isStatic ? b2_staticBody : b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(hWidth * scale / BOX2D_SCALE_FACTOR, hHeight * scale / BOX2D_SCALE_FACTOR);

 	b2FixtureDef fd;
 	fd.shape = &shape;
 	fd.density = 1.0f;
 	fd.friction = 1.0f;

	m_body->CreateFixture(&fd);
}

void PolygonBody::getRect(Rect& rect, const Vector& position, float angle) const
{
	rect.makeInfinite();
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(f->GetShape());
		assert(poly);
		for (size_t i = 0; i < poly->m_vertexCount; ++i)
		{
			const b2Vec2& p = poly->m_vertices[i];
			Vector transform = Math::rotateVector(Vector(p.x, p.y), angle) + position;
			rect.combine(transform);
		}
	}
}
