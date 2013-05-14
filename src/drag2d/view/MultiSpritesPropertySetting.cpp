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

#include "MultiSpritesPropertySetting.h"
#include "PropertySettingPanel.h"

#include <algorithm>

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"

using namespace d2d;

MultiSpritesPropertySetting::MultiSpritesPropertySetting(EditPanel* editPanel, const std::vector<ISprite*>& sprites)
	: IPropertySetting(editPanel, wxT("MultiSprites"))
	, m_sprites(sprites)
{
}

void MultiSpritesPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Align"))->SetValue(wxT("none"));
		pg->GetProperty(wxT("Center"))->SetValue(wxT("none"));
	}
	else
	{
		pg->Clear();

		static const wxChar* align_labels[] = { wxT("none"),
			wxT("left"), wxT("right"), wxT("up"), wxT("down"), NULL };
		pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, align_labels));

		static const wxChar* center_labels[] = { wxT("none"), 
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Center"), wxPG_LABEL, center_labels));
	}
}

void MultiSpritesPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Align"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float left = FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float l = sprite->getPosition().x - sprite->getSymbol().getWidth()*0.5f;
				if (l < left)
					left = l;
			}
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float x = left + sprite->getSymbol().getWidth()*0.5f;
				m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
			}
			m_editPanel->Refresh();
		}
		else if (type == 2)
		{
			float right = -FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float r = sprite->getPosition().x + sprite->getSymbol().getWidth()*0.5f;
				if (r > right)
					right = r;
			}
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float x = right - sprite->getSymbol().getWidth()*0.5f;
				m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
			}
			m_editPanel->Refresh();
		}
		else if (type == 3)
		{
			float up = -FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float u = sprite->getPosition().y + sprite->getSymbol().getHeight()*0.5f;
				if (u > up)
					up = u;
			}
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float y = up - sprite->getSymbol().getHeight()*0.5f;
				m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
			}
			m_editPanel->Refresh();
		}
		else if (type == 4)
		{
			float down = FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float d = sprite->getPosition().y - sprite->getSymbol().getHeight()*0.5f;
				if (d < down)
					down = d;
			}
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float y = down + sprite->getSymbol().getHeight()*0.5f;
				m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
			}
			m_editPanel->Refresh();
		}
	}
	else if (name == wxT("Center"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float left = FLT_MAX, right = -FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float x = sprite->getPosition().x;
				if (x < left) left = x;
				if (x > right) right = x;
			}

			std::vector<ISprite*> sorted(m_sprites);
			std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_x));
			const float space = (right - left) / (sorted.size() - 1);
			for (size_t i = 0, n = sorted.size(); i < n; ++i)
			{
				ISprite* sprite = sorted[i];
				sprite->setTransform(Vector(left + space * i, sprite->getPosition().y), sprite->getAngle());
			}

			m_editPanel->Refresh();
		}
		else if (type == 2)
		{
			float down = FLT_MAX, up = -FLT_MAX;
			for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			{
				ISprite* sprite = m_sprites[i];
				float y = sprite->getPosition().y;
				if (y < down) down = y;
				if (y > up) up = y;
			}

			std::vector<ISprite*> sorted(m_sprites);
			std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_y));
			const float space = (up - down) / (sorted.size() - 1);
			for (size_t i = 0, n = sorted.size(); i < n; ++i)
			{
				ISprite* sprite = sorted[i];
				sprite->setTransform(Vector(sprite->getPosition().x, down + space * i), sprite->getAngle());
			}

			m_editPanel->Refresh();
		}
	}
}

void MultiSpritesPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void MultiSpritesPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		static const wxChar* align_labels[] = { wxT("none"),
			wxT("left"), wxT("right"), NULL };
		pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, align_labels));

		static const wxChar* center_labels[] = { wxT("none"), 
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Center"), wxPG_LABEL, center_labels));
	}

	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("Center"))->Enable(bEnable);
}
