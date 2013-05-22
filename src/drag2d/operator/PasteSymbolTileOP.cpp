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

#include "PasteSymbolTileOP.h"

#include "common/Math.h"
#include "dataset/SpriteFactory.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"
#include "render/SpriteDraw.h"

using namespace d2d;

PasteSymbolTileOP::PasteSymbolTileOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
									 LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
	, m_rotate(0)
{
	m_lastPos.setInvalid();
}

bool PasteSymbolTileOP::onMouseLeftDown(int x, int y)
{
	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_editPanel->transPosScreenToProject(x, y);
		m_lastPos = m_pos;

		ISprite* sprite = SpriteFactory::create(symbol);
		sprite->translate(m_pos);
		sprite->rotate(m_rotate);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return false;
}

bool PasteSymbolTileOP::onMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::onMouseRightDown(x, y)) return true;

	m_rotate += d2d::PI * 0.5f;

	return false;
}

bool PasteSymbolTileOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_lastPos.isValid())
	{
		Vector offset = m_cmpt->getOffset();
		const float dis = offset.length() * 0.5f;
		do
		{
			Vector newPos = d2d::Vector(m_lastPos.x + offset.x, m_lastPos.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x + offset.x, m_lastPos.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x - offset.x, m_lastPos.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x - offset.x, m_lastPos.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x, m_lastPos.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x, m_lastPos.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x - offset.x, m_lastPos.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(m_lastPos.x + offset.x, m_lastPos.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

		} while (0);
	}
	m_editPanel->Refresh();

	return false;
}

bool PasteSymbolTileOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol && m_pos.isValid())
	{
		if (m_pScale)
			SpriteDraw::drawSprite(symbol, m_pos, m_rotate, *m_pScale);
		else
			SpriteDraw::drawSprite(symbol, m_pos, m_rotate);
	}

	return false;
}

bool PasteSymbolTileOP::clear()
{
	if (PasteSymbolOP::clear()) return true;

	m_lastPos.setInvalid();

	return false;
}
