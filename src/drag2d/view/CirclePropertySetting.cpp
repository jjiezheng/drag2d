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

#include "CirclePropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/CircleShape.h"
#include "view/EditPanel.h"

using namespace d2d;

CirclePropertySetting::CirclePropertySetting(EditPanel* editPanel, CircleShape* circle)
	: IPropertySetting(editPanel, wxT("Circle"))
	, m_circle(circle)
{
}

void CirclePropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_circle->name);
		pg->GetProperty(wxT("X"))->SetValue(m_circle->center.x);
		pg->GetProperty(wxT("Y"))->SetValue(m_circle->center.y);
		pg->GetProperty(wxT("Radius"))->SetValue(m_circle->radius);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_circle->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_circle->center.x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_circle->center.y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Radius"), wxPG_LABEL, m_circle->radius));
		pg->SetPropertyAttribute(wxT("Radius"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Radius"), "Precision", 1);
	}
}

void CirclePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_circle->name = wxANY_AS(value, wxString);
	if (name == wxT("X"))
		m_circle->center.x = wxANY_AS(value, float);
	else if (name == wxT("Y"))
		m_circle->center.y = wxANY_AS(value, float);
	else if (name == "Radius")
		m_circle->radius = wxANY_AS(value, float);

	m_editPanel->Refresh();
}

void CirclePropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void CirclePropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_circle->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_circle->center.x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_circle->center.y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Radius"), wxPG_LABEL, m_circle->radius));
		pg->SetPropertyAttribute(wxT("Radius"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Radius"), "Precision", 1);
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Radius"))->Enable(bEnable);
}