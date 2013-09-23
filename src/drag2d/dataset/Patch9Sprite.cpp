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

#include "Patch9Sprite.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

using namespace d2d;

Patch9Sprite::Patch9Sprite()
	: m_symbol(NULL)
{
	width = height = 0;
}

Patch9Sprite::Patch9Sprite(const Patch9Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	width = m_symbol->getWidth();
	height = m_symbol->getHeight();
}

Patch9Sprite::Patch9Sprite(Patch9Symbol* symbol)
	: m_symbol(symbol)
{
	width = m_symbol->getWidth();
	height = m_symbol->getHeight();
	buildBounding();
}

Patch9Sprite::~Patch9Sprite()
{
}

Patch9Sprite* Patch9Sprite::clone()
{
	return new Patch9Sprite(*this);
}

const Patch9Symbol& Patch9Sprite::getSymbol() const
{
	return *m_symbol;
}

void Patch9Sprite::setSymbol(ISymbol* symbol)
{
	Patch9Symbol* anim = dynamic_cast<Patch9Symbol*>(symbol);
	if (anim) m_symbol = anim;
}

void Patch9Sprite::loadBodyFromFile()
{
}

void Patch9Sprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		m_bounding->combine(Rect(m_pos, m_symbol->getWidth() * m_xScale * 0.5f,
			m_symbol->getHeight() * m_yScale * 0.5f));
		m_bounding->rotate(m_angle);
	}
}