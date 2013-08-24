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

#include "FontBlankSprite.h"

#include "common/Rect.h"
#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

using namespace d2d;

FontBlankSprite::FontBlankSprite()
	: m_symbol(NULL)
{
}

FontBlankSprite::FontBlankSprite(const FontBlankSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

FontBlankSprite::FontBlankSprite(FontBlankSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

FontBlankSprite::~FontBlankSprite()
{
}

FontBlankSprite* FontBlankSprite::clone()
{
	return new FontBlankSprite(*this);
}

const FontBlankSymbol& FontBlankSprite::getSymbol() const
{
	return *m_symbol;
}

void FontBlankSprite::setSymbol(ISymbol* symbol)
{
	FontBlankSymbol* font = dynamic_cast<FontBlankSymbol*>(symbol);
	if (font) m_symbol = font;
}

void FontBlankSprite::loadBodyFromFile()
{
}

void FontBlankSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		m_bounding->combine(Rect(m_pos, m_symbol->width * m_xScale * 0.5f,
			m_symbol->height * m_yScale * 0.5f));
		m_bounding->rotate(m_angle);
	}
}