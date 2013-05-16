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

#include "PasteSymbolOP.h"

#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/MultiSpritesImpl.h"
#include "render/SpriteDraw.h"

using namespace d2d;

PasteSymbolOP::PasteSymbolOP(EditPanel* editPanel, MultiSpritesImpl* panelImpl, 
							 LibraryPanel* libraryPanel, float* pScale/* = NULL*/)
	: ZoomViewOP(editPanel, true)
	, m_panelImpl(panelImpl)
	, m_libraryPanel(libraryPanel)
	, m_pScale(pScale)
{
	m_pos.setInvalid();
}

bool PasteSymbolOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol)
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::create(symbol);
		sprite->translate(m_pos);
		if (m_pScale)
			sprite->setScale(*m_pScale);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return false;
}

bool PasteSymbolOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_pos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool PasteSymbolOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol && m_pos.isValid())
	{
		if (m_pScale)
			SpriteDraw::drawSprite(symbol, m_pos, 0.0f, *m_pScale);
		else
			SpriteDraw::drawSprite(symbol, m_pos);
	}

	return false;
}

bool PasteSymbolOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_pos.setInvalid();

	return false;
}
