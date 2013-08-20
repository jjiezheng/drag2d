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

#include "ComplexSprite.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

using namespace d2d;

ComplexSprite::ComplexSprite()
	: m_symbol(NULL)
{
}

ComplexSprite::ComplexSprite(const ComplexSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

ComplexSprite::ComplexSprite(ComplexSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();	
}

ComplexSprite::~ComplexSprite()
{
}

ComplexSprite* ComplexSprite::clone()
{
	return new ComplexSprite(*this);
}

const ComplexSymbol& ComplexSprite::getSymbol() const
{
	return *m_symbol;
}

void ComplexSprite::setSymbol(ISymbol* symbol)
{
	ComplexSymbol* complex = dynamic_cast<ComplexSymbol*>(symbol);
	if (complex) m_symbol = complex;
}

void ComplexSprite::loadBodyFromFile()
{
}

void ComplexSprite::refreshBounding()
{
	buildBounding();
}

void ComplexSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect& rect = m_symbol->m_rect;

		rect.scale(m_xScale, m_yScale);
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->rotate(m_angle);
	}
}
