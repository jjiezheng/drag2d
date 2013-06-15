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

#include "RectPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/RectShape.h"
#include "view/EditPanel.h"

using namespace d2d;

RectPropertySetting::RectPropertySetting(EditPanel* editPanel, RectShape* rect)
	: IPropertySetting(editPanel, wxT("Rect"))
	, m_rect(rect)
{
}

void RectPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_rect->name);
		pg->GetProperty(wxT("X"))->SetValue(m_rect->m_rect.xCenter());
		pg->GetProperty(wxT("Y"))->SetValue(m_rect->m_rect.yCenter());
		pg->GetProperty(wxT("Half Width"))->SetValue(m_rect->m_rect.xLength()*0.5f);
		pg->GetProperty(wxT("Half Height"))->SetValue(m_rect->m_rect.xLength()*0.5f);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_rect->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_rect->m_rect.xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_rect->m_rect.yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Half Width"), wxPG_LABEL, m_rect->m_rect.xLength()*0.5f));
		pg->SetPropertyAttribute(wxT("Half Width"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Half Width"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Half Height"), wxPG_LABEL, m_rect->m_rect.yLength()*0.5f));
		pg->SetPropertyAttribute(wxT("Half Height"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Half Height"), "Precision", 1);
	}
}

void RectPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
	{
		m_rect->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float hWidth = m_rect->m_rect.xLength() * 0.5f;
		m_rect->m_rect.xMin = x - hWidth;
		m_rect->m_rect.xMax = x + hWidth;
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float hHeight = m_rect->m_rect.yLength() * 0.5f;
		m_rect->m_rect.yMin = y - hHeight;
		m_rect->m_rect.yMax = y + hHeight;
	}
	else if (name == wxT("Half Width"))
	{
		const float x = m_rect->m_rect.xCenter();
		const float hWidth = wxANY_AS(value, float);
		m_rect->m_rect.xMin = x - hWidth;
		m_rect->m_rect.xMax = x + hWidth;
	}
	else if (name == wxT("Half Height"))
	{
		const float y = m_rect->m_rect.yCenter();
		const float hHeight = wxANY_AS(value, float);
		m_rect->m_rect.yMin = y - hHeight;
		m_rect->m_rect.yMax = y + hHeight;
	}

	m_editPanel->Refresh();
}

void RectPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void RectPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_rect->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_rect->m_rect.xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_rect->m_rect.yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Half Width"), wxPG_LABEL, m_rect->m_rect.xLength()*0.5f));
		pg->SetPropertyAttribute(wxT("Half Width"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Half Width"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Half Height"), wxPG_LABEL, m_rect->m_rect.yLength()*0.5f));
		pg->SetPropertyAttribute(wxT("Half Height"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Half Height"), "Precision", 1);
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Half Width"))->Enable(bEnable);
	pg->GetProperty(wxT("Half Height"))->Enable(bEnable);
}
