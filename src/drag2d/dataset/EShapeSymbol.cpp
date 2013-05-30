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
#include "dataset/IShape.h"

using namespace d2d;

EShapeSymbol::EShapeSymbol()
{
}

EShapeSymbol::EShapeSymbol(const EShapeSymbol& symbol)
{
	m_filepath = symbol.m_filepath;
	m_shapes.reserve(symbol.m_shapes.size());
	for (size_t i = 0, n = symbol.m_shapes.size(); i < n; ++i)
		m_shapes.push_back(symbol.m_shapes[i]->clone());
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
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->draw();
}

float EShapeSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float EShapeSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void EShapeSymbol::loadResources()
{
	clear();

	EShapeFileAdapter adpater(m_shapes);
	adpater.load(m_filepath.c_str());
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->retain();
}

void EShapeSymbol::clear()
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}
