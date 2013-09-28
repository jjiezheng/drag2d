#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "dataset/TextSprite.h"
#include "dataset/Patch9Sprite.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/Scale9PropertySetting.h"
#include "view/MultiSpritesImpl.h"
#include "render/DrawSelectedSpriteVisitor.h"

namespace d2d
{

SelectSpritesOP::SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
								 PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback/* = NULL*/,
								 const Colorf& color /*= Colorf(0, 0, 0)*/)
	: DrawRectangleOP(editPanel, color)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_lastCtrlPress(false)
	, m_bDraggable(true)
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
	else if (wxGetKeyState(WXK_CONTROL_X))
	{
		clearClipboard();
		m_selection->traverse(FetchAllVisitor<ISprite>(m_clipboard));
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
			m_clipboard[i]->retain();
		m_spritesImpl->removeSpriteSelection();
	}
	else if (m_lastCtrlPress && (keyCode == 'c' || keyCode == 'C')/*wxGetKeyState(WXK_CONTROL_C)*/)
	{
		clearClipboard();

		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			m_clipboard.push_back(sprites[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL_V))
	{
		// todo: should deep copy
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			m_clipboard[i]->retain();
			m_spritesImpl->insertSprite(m_clipboard[i]);
			m_editPanel->Refresh();
		}
	}

	m_lastCtrlPress = keyCode == WXK_CONTROL;

	return false;
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	m_bDraggable = true;

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
						m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
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
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectSpritesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

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
				m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

	return false;
}

bool SelectSpritesOP::onMouseRightDown(int x, int y)
{
	if (DrawRectangleOP::onMouseRightDown(x, y)) return true;

	return SelectSpritesOP::onMouseLeftDown(x, y);
}

bool SelectSpritesOP::onMouseDrag(int x, int y)
{
	if (DrawRectangleOP::onMouseDrag(x, y)) return true;

	return !m_bDraggable;
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
//  	else if (Patch9Sprite* scale = dynamic_cast<Patch9Sprite*>(sprite))
//  		return new Scale9PropertySetting(m_editPanel, const_cast<Patch9Symbol*>(&scale->getSymbol()));
	else if (sprite)
		return new SpritePropertySetting(m_editPanel, sprite);
	else 
		return NULL;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ISprite*>& sprites) const
{
	return new MultiSpritesPropertySetting(m_editPanel, sprites);
}

void SelectSpritesOP::clearClipboard()
{
	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		m_clipboard[i]->release();
	m_clipboard.clear();
}

} // d2d