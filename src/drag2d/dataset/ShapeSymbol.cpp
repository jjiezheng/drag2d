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

#include "ShapeSymbol.h"

#include "dataset/PolygonShape.h"
#include "common/ShapeFileAdapter.h"

using namespace d2d;

ShapeSymbol::ShapeSymbol()
{
}

ShapeSymbol::ShapeSymbol(const ShapeSymbol& symbol)
{
	m_filepath = symbol.m_filepath;
	m_shapes.reserve(symbol.m_shapes.size());
	for (size_t i = 0, n = symbol.m_shapes.size(); i < n; ++i)
		m_shapes.push_back(symbol.m_shapes[i]->clone());
}

ShapeSymbol::~ShapeSymbol()
{
	clear();
}

ShapeSymbol* ShapeSymbol::clone()
{
	return new ShapeSymbol(*this); 
}

void ShapeSymbol::loadFromTextFile(std::ifstream& fin)
{

}

void ShapeSymbol::storeToTextFile(std::ofstream& fout) const
{

}

void ShapeSymbol::reloadTexture() const
{

}

void ShapeSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->draw();
}

float ShapeSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float ShapeSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void ShapeSymbol::loadResources()
{
	clear();

	ShapeFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	for (size_t i = 0, n = adapter.m_layers.size(); i < n; ++i)
	{
		for (size_t j = 0, m = adapter.m_layers[i]->polys.size(); j < m; ++j)
		{
			adapter.m_layers[i]->polys[j]->retain();
			m_shapes.push_back(adapter.m_layers[i]->polys[j]);
		}
	}
}

void ShapeSymbol::clear()
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}
