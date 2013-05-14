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

#include "BodyFactory.h"
#include "BodyData.h"
#include "PolygonBody.h"
#include "CircleBody.h"
#include "ComplexBody.h"

using namespace d2d;

b2World* BodyFactory::m_world = NULL;

IBody* BodyFactory::createBody(const wxString& filename, float scale/* = 1.0f*/)
{
	IBody* body = NULL;
	BodyData* bd = BodyDataMgr::Instance()->getItem(filename);
	switch (bd->type())
	{
	case BodyData::e_polygon:
		body = new PolygonBody(m_world, *bd, scale);
		break;
	case BodyData::e_circle:
		body = new CircleBody(m_world, *bd, scale);
	}
	return body;
}

// todo: BodyData's destructor func will delete the FixtureData pointer
IBody* BodyFactory::createBody(const std::vector<Vector>& triangles, float scale/* = 1.0f*/)
{
	BodyData bd;

	size_t index = 0;
	for (size_t i = 0, n = triangles.size() / 3; i < n; ++i)
	{
		BodyData::FixtureData* fd = new BodyData::FixtureData;
		fd->vertices.push_back(triangles[index++]);
		fd->vertices.push_back(triangles[index++]);
		fd->vertices.push_back(triangles[index++]);
		bd.insertFixture(fd);
	}

	return new PolygonBody(m_world, bd, scale);
}

IBody* BodyFactory::createBody(const std::vector<FixtureDataInfo>& fixtures, float scale /*= 1.0f*/)
{
	return new ComplexBody(m_world, fixtures, scale);
}
