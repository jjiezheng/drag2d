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

#include "PasteSymbolRandomOP.h"

#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/MultiSpritesImpl.h"
#include "render/SpriteDraw.h"

using namespace d2d;

PasteSymbolRandomOP::PasteSymbolRandomOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
										 LibraryPanel* libraryPanel, PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolRandomOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::create(symbol);
		sprite->translate(m_pos);
		if (m_randomValue.scale != 1.0f) 
			sprite->setScale(m_randomValue.scale);
		if (m_randomValue.angle != 0.0f) 
			sprite->setTransform(m_pos, m_randomValue.angle);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::onMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::onMouseRightDown(x, y)) return true;

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->getSymbol();
	if (symbol && m_pos.isValid())
		SpriteDraw::drawSprite(symbol, m_pos, m_randomValue.angle, m_randomValue.scale);

	return false;
}

void PasteSymbolRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}
