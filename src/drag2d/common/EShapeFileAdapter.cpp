/*
* Copyright (c) 2013 Guang Zhu http://runnersoft.net
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

#include "EShapeFileAdapter.h"

#include "common/Vector.h"
#include "dataset/PolygonShape.h"
#include "dataset/ChainShape.h"
#include "dataset/RectShape.h"
#include "dataset/CircleShape.h"

using namespace d2d;

EShapeFileAdapter::EShapeFileAdapter(std::vector<IShape*>& shapes)
	: m_shapes(shapes)
{
}

void EShapeFileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value shapeValue = value["shapes"][i++];
	while (!shapeValue.isNull()) {
		IShape* shape = loadShape(shapeValue);
		m_shapes.push_back(shape);
		shapeValue = value["shapes"][i++];
	}
}

void EShapeFileAdapter::store(const char* filename)
{
	Json::Value value;

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		value["shapes"][i] = store(m_shapes[i]);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

IShape* EShapeFileAdapter::loadShape(const Json::Value& value)
{
	IShape* shape = NULL;

	if (value.isNull())
		;
	else if (!value["polygon"].isNull())
		shape = loadPolygon(value["polygon"]);
	else if (!value["chain"].isNull())
		shape = loadChain(value["chain"]);
	else if (!value["rect"].isNull())
		shape = loadRect(value["rect"]);
	else if (!value["circle"].isNull())
		shape = loadCircle(value["circle"]);

	return shape;
}

Json::Value EShapeFileAdapter::store(IShape* shape)
{
	Json::Value value;

	if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
		value["polygon"] = store(poly);
	else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape))
		value["chain"] = store(chain);
	else if (RectShape* rect = dynamic_cast<RectShape*>(shape))
		value["rect"] = store(rect);
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		value["circle"] = store(circle);

	return value;
}

IShape* EShapeFileAdapter::loadPolygon(const Json::Value& value)
{
	std::vector<Vector> vertices;
	size_t num = value["vertices"]["x"].size();
	vertices.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		vertices[i].x = value["vertices"]["x"][i].asDouble();
		vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	return new PolygonShape(vertices);
}

IShape* EShapeFileAdapter::loadChain(const Json::Value& value)
{
	std::vector<Vector> vertices;
	size_t num = value["vertices"]["x"].size();
	vertices.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		vertices[i].x = value["vertices"]["x"][i].asDouble();
		vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	bool isLoop = value["closed"].asBool();

	return new ChainShape(vertices, isLoop);
}

IShape* EShapeFileAdapter::loadRect(const Json::Value& value)
{
	const float xmin = value["xmin"].asDouble(),
		xmax = value["xmax"].asDouble(),
		ymin = value["ymin"].asDouble(),
		ymax = value["ymax"].asDouble();

	return new RectShape(Vector(xmin, ymin), Vector(xmax, ymax));
}

IShape* EShapeFileAdapter::loadCircle(const Json::Value& value)
{
	const float x = value["x"].asDouble(),
		y = value["y"].asDouble(),
		radius = value["radius"].asDouble();

	return new CircleShape(Vector(x, y), radius);
}

Json::Value EShapeFileAdapter::store(const PolygonShape* poly)
{
	Json::Value value;

	const std::vector<Vector>& vertices = poly->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}

	return value;
}

Json::Value EShapeFileAdapter::store(const ChainShape* chain)
{
	Json::Value value;

	const std::vector<Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}
	value["closed"] = chain->isClosed();

	return value;
}

Json::Value EShapeFileAdapter::store(const RectShape* rect)
{
	Json::Value value;

	value["xmin"] = rect->m_rect.xMin;
	value["xmax"] = rect->m_rect.xMax;
	value["ymin"] = rect->m_rect.yMin;
	value["ymax"] = rect->m_rect.yMax;

	return value;
}

Json::Value EShapeFileAdapter::store(const CircleShape* circle)
{
	Json::Value value;

	value["x"] = circle->center.x;
	value["y"] = circle->center.y;
	value["radius"] = circle->radius;

	return value;
}