/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#include "PasteSpriteOP.h"

#include "common/visitors.h"
#include "component/PasteSpriteCMPT.h"
#include "view/SpritesPanelImpl.h"
#include "render/GL10.h"
#include "render/SpriteDraw.h"

using namespace d2d;

PasteSpriteOP::PasteSpriteOP(EditPanel* editPanel, SpritesPanelImpl* spritesImpl,
							 PropertySettingPanel* propertyPanel, PasteSpriteCMPT* cmpt /*= NULL*/)
	: SelectSpritesOP(editPanel, spritesImpl, propertyPanel)
	, m_spritesImpl(spritesImpl)
	, m_cmpt(cmpt)
{
	m_selection = spritesImpl->getSpriteSelection();
	m_selection->retain();

	m_pos.setInvalid();
}

PasteSpriteOP::~PasteSpriteOP()
{
	m_selection->release();
}

bool PasteSpriteOP::onKeyDown(int keyCode)
{
	if (SelectSpritesOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_batch.clear();

	return false;
}

bool PasteSpriteOP::onMouseLeftDown(int x, int y)
{
	if (m_batch.empty())
	{
		if (SelectSpritesOP::onMouseLeftDown(x, y))
			return true;
	}
	else
	{
		if (ZoomViewOP::onMouseLeftDown(x, y))
			return true;
	}

	setMousePos(x, y);

	if (m_cmpt)
		m_batch.insertToSpritesImpl(m_spritesImpl, m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.insertToSpritesImpl(m_spritesImpl, m_pos, false, false);
	m_batch.loadFromSelection(*m_selection);

	return false;
}

bool PasteSpriteOP::onMouseLeftUp(int x, int y)
{
	if (m_batch.empty())
	{
		if (SelectSpritesOP::onMouseLeftUp(x, y))
			return true;
	}
	else
	{
		if (ZoomViewOP::onMouseLeftUp(x, y))
			return true;
	}

	setMousePos(x, y);

	m_batch.loadFromSelection(*m_selection);

	return false;
}

bool PasteSpriteOP::onMouseRightDown(int x, int y)
{
	if (SelectSpritesOP::onMouseRightDown(x, y)) return true;

	clear();

	return false;
}

bool PasteSpriteOP::onMouseMove(int x, int y)
{
	if (SelectSpritesOP::onMouseMove(x, y)) return true;

	setMousePos(x, y);

	m_editPanel->Refresh();

	return false;
}

bool PasteSpriteOP::onDraw() const
{
	if (SelectSpritesOP::onDraw()) return true;

	if (m_cmpt)
		m_batch.draw(m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.draw(m_pos, false, false);

	return false;
}

bool PasteSpriteOP::clear()
{
	if (SelectSpritesOP::clear()) return true;

	m_pos.setInvalid();
	m_batch.clear();

	return false;
}

void PasteSpriteOP::setMousePos(int x, int y)
{
	m_pos = m_editPanel->transPosScreenToProject(x, y);
	if (wxGetKeyState(WXK_SHIFT))
		fixPosOrthogonal();
}

void PasteSpriteOP::fixPosOrthogonal()
{
	const Vector& center = m_batch.getCenter();
	if (m_pos.isValid() && center.isValid())
	{
		if (fabs(m_pos.x - center.x) < fabs(m_pos.y - center.y))
			m_pos.x = center.x;
		else
			m_pos.y = center.y;
	}
}

//////////////////////////////////////////////////////////////////////////
// class PasteSpriteOP::SpriteBatch
//////////////////////////////////////////////////////////////////////////

PasteSpriteOP::SpriteBatch::
SpriteBatch()
{
	m_center.setInvalid();
}

PasteSpriteOP::SpriteBatch::
~SpriteBatch()
{
	clear();
}

void PasteSpriteOP::SpriteBatch::
loadFromSelection(const SpriteSelection& selection)
{
	if (!selection.empty())
	{	
		clear();
		selection.traverse(FetchAllVisitor<ISprite>(m_selected));
		computeCenter();
	}
}

void PasteSpriteOP::SpriteBatch::
insertToSpritesImpl(SpritesPanelImpl* spritesImpl, const Vector& pos,
					bool isHorMirror, bool isVerMirror)
{
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
	{
		ISprite* sprite = m_selected[i];
		Vector fixed = sprite->getPosition() - m_center;
		if (isHorMirror)
			fixed.x += (m_center.x - sprite->getPosition().x) * 2;
		if (isVerMirror)
			fixed.y += (m_center.y - sprite->getPosition().y) * 2;

//		spritesImpl->insertSprite(sprite->getSymbol(), fixed + pos);

		ISprite* newOne = sprite->clone();
		newOne->setTransform(fixed + pos, newOne->getAngle());
		spritesImpl->insertSprite(newOne);
		newOne->release();
	}
}

void PasteSpriteOP::SpriteBatch::
draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const
{
	if (!m_selected.empty() && pos.isValid())
	{
		const float xOffset = pos.x - m_center.x,
			yOffset = pos.y - m_center.y;

		for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		{
			const Vector& pos = m_selected[i]->getPosition();

			float x = xOffset, y = yOffset;
			if (isHorMirror)
				x += (m_center.x - pos.x) * 2;
			if (isVerMirror)
				y += (m_center.y - pos.y) * 2;

			GL10::PushMatrix();
			GL10::Translatef(x, y, 0.0f);
			SpriteDraw::drawSprite(m_selected[i]);
			GL10::PopMatrix();
		}
	}
}

void PasteSpriteOP::SpriteBatch::
clear()
{
	m_selected.clear();
	m_center.setInvalid();
}

void PasteSpriteOP::SpriteBatch::
computeCenter()
{
	m_center.set(0, 0);
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		m_center += m_selected[i]->getPosition();
	m_center /= m_selected.size();
}
