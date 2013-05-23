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

#include "TextPropertySetting.h"
#include "PropertySettingPanel.h"

#include <wx/propgrid/advprops.h>

#include "dataset/TextSprite.h"
#include "view/EditPanel.h"

using namespace d2d;

TextPropertySetting::TextPropertySetting(EditPanel* editPanel, TextSprite* sprite)
	: IPropertySetting(editPanel, wxT("Text Sprite"))
	, m_sprite(sprite)
{
}

void TextPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Text"))->SetValue(m_sprite->getText());

		pg->GetProperty(wxT("Size"))->SetValue(m_sprite->getSize());

		const d2d::Colori& c = m_sprite->getColor();
		wxString str = "("+wxString::FromDouble(c.r)+","+wxString::FromDouble(c.g)+","+wxString::FromDouble(c.b)+","+wxString::FromDouble(c.a)+")";
		pg->SetPropertyValueString(wxT("Color"), str);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Text"), wxPG_LABEL, m_sprite->getText()));

		pg->Append(new wxFloatProperty(wxT("Size"), wxPG_LABEL, m_sprite->getSize()));
		pg->SetPropertyAttribute(wxT("Size"), "Precision", 0);

		const d2d::Colori& c = m_sprite->getColor();
		pg->Append(new wxColourProperty("Color", wxPG_LABEL, wxColour(c.r, c.g, c.b, c.a)));
		pg->SetPropertyAttribute("Color", "HasAlpha", false);
	}
}

void TextPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == "Text")
	{
		wxString text = wxANY_AS(value, wxString);
		m_sprite->setText(text.ToStdString());
	}
	else if (name == "Size")
	{
		m_sprite->setSize(wxANY_AS(value, float));
	}
	else if (name == "Color")
	{
		wxColour c = wxANY_AS(value, wxColour);
		m_sprite->setColor(d2d::Colori(c.Red(), c.Green(), c.Blue(), c.Alpha()));
	}

	m_editPanel->Refresh();
}

void TextPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void TextPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Text"), wxPG_LABEL, m_sprite->getText()));

		pg->Append(new wxFloatProperty(wxT("Size"), wxPG_LABEL, m_sprite->getSize()));

		const d2d::Colori& c = m_sprite->getColor();
		pg->Append(new wxColourProperty("Color", wxPG_LABEL, wxColour(c.r, c.g, c.b, c.a)));
		pg->SetPropertyAttribute("Color", "HasAlpha", false);
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Text"))->Enable(bEnable);
	pg->GetProperty(wxT("Size"))->Enable(bEnable);
	pg->GetProperty(wxT("Color"))->Enable(bEnable);
}
