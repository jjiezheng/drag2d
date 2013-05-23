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

#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "dataset/TextSprite.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/MultiSpritesImpl.h"
#include "render/DrawSelectedSpriteVisitor.h"

using namespace d2d;

SelectSpritesOP::SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
								 PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback/* = NULL*/,
								 const Colorf& color /*= Colorf(0, 0, 0)*/)
	: DrawRectangleOP(editPanel, color)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
{
	m_selection = spritesImpl->getSpriteSelection();
	m_selection->retain();

	m_firstPos.setInvalid();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->clear();
	m_selection->release();
}

bool SelectSpritesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_spritesImpl->removeSpriteSelection();	

	return false;
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->isExist(selected))
				m_selection->erase(selected);
			else
			{
				m_selection->insert(selected);
				if (m_propertyPanel)
				{
					if (m_selection->size() == 1)
						m_propertyPanel->setPropertySetting(createPropertySetting(selected));
					else if (m_selection->size() > 1)
					{
						std::vector<ISprite*> sprites;
						m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
						m_propertyPanel->setPropertySetting(createPropertySetting(sprites));
					}
					else
						m_propertyPanel->setPropertySetting(NULL);
				}
			}
		}
		else
		{
			if (!m_selection->isExist(selected))
			{
				m_selection->clear();
				m_selection->insert(selected);
				if (m_propertyPanel)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		m_selection->clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectSpritesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		Rect rect(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		std::vector<ISprite*> sprites;
		m_spritesImpl->querySpritesByRect(rect, sprites);
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			m_selection->insert(sprites[i]);

		if (m_propertyPanel)
		{
			if (m_selection->size() == 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(sprites[0]));
			else if (m_selection->size() > 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(sprites));
			else
				m_propertyPanel->setPropertySetting(NULL);
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

	return false;
}

bool SelectSpritesOP::onDraw() const
{
	if (DrawRectangleOP::onDraw()) return true;

	m_selection->traverse(DrawSelectedSpriteVisitor(Colorf(1, 0, 0)));

	return false;
}

bool SelectSpritesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	m_selection->clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(ISprite* sprite) const
{
	if (TextSprite* text = dynamic_cast<TextSprite*>(sprite))
		return new TextPropertySetting(m_editPanel, text);
	else
		return new SpritePropertySetting(m_editPanel, sprite);
}

IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ISprite*>& sprites) const
{
	return new MultiSpritesPropertySetting(m_editPanel, sprites);
}
