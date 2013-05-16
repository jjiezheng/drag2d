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

#include "PasteSymbolCaptureOP.h"

#include "common/Math.h"
#include "dataset/SpriteFactory.h"
#include "component/PasteSymbolCaptureCMPT.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"

using namespace d2d;

PasteSymbolCaptureOP::PasteSymbolCaptureOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
										   LibraryPanel* libraryPanel, PasteSymbolCaptureCMPT* cmpt)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_lastPos.setInvalid();
}

bool PasteSymbolCaptureOP::onMouseLeftDown(int x, int y)
{
	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_editPanel->transPosScreenToProject(x, y);
		m_lastPos = m_pos;

		ISprite* sprite = SpriteFactory::create(symbol);
		sprite->translate(m_pos);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return false;
}

bool PasteSymbolCaptureOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_lastPos.isValid())
	{
		Vector offset = m_cmpt->getOffset();
		Vector newPos = m_lastPos + offset;
		if (Math::getDistance(m_pos, newPos) < 
			Math::getDistance(offset, Vector(0, 0)))
		{
			m_bCaptured = true;
			m_pos = newPos;
		}
	}
	m_editPanel->Refresh();

	return false;
}

bool PasteSymbolCaptureOP::clear()
{
	if (PasteSymbolOP::clear()) return true;

	m_lastPos.setInvalid();

	return false;
}
