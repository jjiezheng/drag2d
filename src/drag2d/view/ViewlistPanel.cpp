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

#include "ViewlistPanel.h"
#include "ViewlistList.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/SelectionSet.h"
#include "widgets/VerticalImageList.h"
#include "view/EditPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/PropertySettingPanel.h"
#include "view/MultiSpritesImpl.h"

using namespace d2d;

ViewlistPanel::ViewlistPanel(wxWindow* parent, EditPanel* editPanel,
							 MultiSpritesImpl* stage /*= NULL*/, 
							 PropertySettingPanel* property /*= NULL*/)
	: wxPanel(parent, wxID_ANY)
	, m_editPanel(editPanel)
	, m_stage(stage)
	, m_property(property)
{
	initLayout();
}

void ViewlistPanel::remove(ISprite* sprite)
{
	size_t i = 0;
	for (size_t n = m_sprites.size(); i < n; ++i)
		if (m_sprites[i] == sprite)
			break;
	assert(i != m_sprites.size());

	m_list->remove(i);
	m_sprites.erase(m_sprites.begin() + i);
}

void ViewlistPanel::insert(ISprite* sprite)
{
	m_list->insert(const_cast<ISymbol*>(&sprite->getSymbol()));
	m_sprites.push_back(sprite);
}

void ViewlistPanel::onSelected(int index)
{
	if (m_property)
	{
		d2d::IPropertySetting* setting = new d2d::SpritePropertySetting(m_editPanel, m_sprites[index]);
		m_property->setPropertySetting(setting);
	}

	if (m_stage)
	{
		d2d::SpriteSelection* selection = m_stage->getSpriteSelection();
		selection->clear();
		selection->insert(m_sprites[index]);
		m_editPanel->Refresh();
	}
}

void ViewlistPanel::clear()
{
	m_list->clear();
	m_sprites.clear();
}

void ViewlistPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_list = new ViewlistList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}