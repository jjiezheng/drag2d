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

#include "BezierPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/BezierShape.h"
#include "view/EditPanel.h"

using namespace d2d;

BezierPropertySetting::BezierPropertySetting(EditPanel* editPanel, BezierShape* bezier)
	: IPropertySetting(editPanel, wxT("Bezier"))
	, m_bezier(bezier)
{
}

void BezierPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_bezier->name);
		pg->GetProperty(wxT("X"))->SetValue(m_bezier->getRect().xCenter());
		pg->GetProperty(wxT("Y"))->SetValue(m_bezier->getRect().yCenter());
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_bezier->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_bezier->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_bezier->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);
	}
}

void BezierPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
	{
		m_bezier->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_bezier->getRect().xCenter();
		std::vector<Vector> vertices = m_bezier->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].x += dx;
		m_bezier->setVertices(vertices);
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_bezier->getRect().yCenter();
		std::vector<Vector> vertices = m_bezier->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].y += dy;
		m_bezier->setVertices(vertices);
	}

	m_editPanel->Refresh();
}

void BezierPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void BezierPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_bezier->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_bezier->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_bezier->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
}
