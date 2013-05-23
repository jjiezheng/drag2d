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

#include "TextSprite.h"

#include "common/Rect.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

using namespace d2d;

TextSprite::TextSprite()
	: m_symbol(NULL)
{
}

TextSprite::TextSprite(const TextSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

TextSprite::TextSprite(FontSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

TextSprite* TextSprite::clone()
{
	return new TextSprite(*this);
}

const FontSymbol& TextSprite::getSymbol() const
{
	return *m_symbol;
}

void TextSprite::setSymbol(ISymbol* symbol)
{
	FontSymbol* font = dynamic_cast<FontSymbol*>(symbol);
	if (font) m_symbol = font;
}

void TextSprite::loadBodyFromFile()
{
}

void TextSprite::setText(const std::string& text)
{
	m_text = text;
	buildBounding();
}

void TextSprite::buildBounding()
{
	if (!m_symbol) return;

	const float width = m_symbol->getWidth(this) * m_scale,
		height = m_symbol->getHeight(this) * m_scale;

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_aabb);
	Rect rect;
	rect.xMin = 0;
	rect.xMax = width;
	rect.yMin = 0;
	rect.yMax = height;
	rect.translate(m_pos);
	m_bounding->initFromRect(rect);
}