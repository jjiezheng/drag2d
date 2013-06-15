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

#include "EShapeSymbol.h"

#include "common/EShapeFileAdapter.h"
#include "dataset/BezierShape.h"
#include "dataset/PolygonShape.h"
#include "dataset/ChainShape.h"
#include "dataset/RectShape.h"
#include "dataset/CircleShape.h"
#include "dataset/Bitmap.h"

using namespace d2d;

const float EShapeSymbol::SCALE = 0.5f;

EShapeSymbol::EShapeSymbol()
{
}

EShapeSymbol::EShapeSymbol(const EShapeSymbol& symbol)
{
	m_filepath = symbol.m_filepath;
	shapes.reserve(symbol.shapes.size());
	for (size_t i = 0, n = symbol.shapes.size(); i < n; ++i)
		shapes.push_back(symbol.shapes[i]->clone());
}

EShapeSymbol::~EShapeSymbol()
{
	clear();
}

EShapeSymbol* EShapeSymbol::clone()
{
	return new EShapeSymbol(*this); 
}

void EShapeSymbol::loadFromTextFile(std::ifstream& fin)
{

}

void EShapeSymbol::storeToTextFile(std::ofstream& fout) const
{

}

void EShapeSymbol::reloadTexture() const
{

}

void EShapeSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->draw();
}

float EShapeSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float EShapeSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void EShapeSymbol::refresh()
{
	loadThumbnail();
}

void EShapeSymbol::loadResources()
{
	clear();

	EShapeFileAdapter adpater(shapes);
	adpater.load(m_filepath.c_str());

	loadThumbnail();
}

void EShapeSymbol::loadThumbnail()
{
	if (shapes.empty()) return;

	static const int PADDING = 2;

	Rect rect;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		rect.combine(shapes[i]->getRect());

	if (m_bitmap) delete m_bitmap;
	m_bitmap = new Bitmap(
		new wxBitmap(
		rect.xLength() * SCALE + PADDING, 
		rect.yLength() * SCALE + PADDING)
		);

	const float dx = (-rect.xMin * SCALE) + PADDING*0.5f,
		dy = (rect.yMax * SCALE) + PADDING*0.5f;

	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.Clear();

	for (size_t i = 0, n = shapes.size(); i < n ;++i)
	{
		IShape* shape = shapes[i];
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(shape))
		{
			size_t size = bezier->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = bezier->getVertices()[i].x * SCALE + dx;
				points[i].y = -bezier->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawLines(size, points);
			delete points;
		}
		else if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
		{
			size_t size = poly->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = poly->getVertices()[i].x * SCALE + dx;
				points[i].y = -poly->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawPolygon(size, points);
			delete points;
		}
		else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape))
		{
			size_t size = chain->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = chain->getVertices()[i].x * SCALE + dx;
				points[i].y = -chain->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawLines(size, points);
			delete points;
		}
		else if (RectShape* rect = dynamic_cast<RectShape*>(shape))
		{
			memDC.DrawRectangle(
				rect->m_rect.xMin * SCALE + dx,
				-rect->m_rect.yMax * SCALE + dy,
				rect->m_rect.xLength() * SCALE,
				rect->m_rect.yLength() * SCALE);
		}
		else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		{
			memDC.DrawCircle(wxPoint(circle->center.x * SCALE + dx, -circle->center.y * SCALE + dy), 
				circle->radius * SCALE);
		}
	}

	memDC.SelectObject(wxNullBitmap);
}

void EShapeSymbol::clear()
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->release();
	shapes.clear();
}
