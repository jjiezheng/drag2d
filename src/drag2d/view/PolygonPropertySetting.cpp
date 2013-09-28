#include "PolygonPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/PolygonShape.h"
#include "view/EditPanel.h"

namespace d2d
{

PolygonPropertySetting::PolygonPropertySetting(EditPanel* editPanel, PolygonShape* poly)
	: IPropertySetting(editPanel, wxT("Polygon"))
	, m_poly(poly)
{
}

void PolygonPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_poly->name);
		pg->GetProperty(wxT("X"))->SetValue(m_poly->getRect().xCenter());
		pg->GetProperty(wxT("Y"))->SetValue(m_poly->getRect().yCenter());
		pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_poly->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_poly->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_poly->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		static const wxChar* mirror_labels[] = { wxT("none"),
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
	}
}

void PolygonPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
	{
		m_poly->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_poly->getRect().xCenter();
		std::vector<Vector> vertices = m_poly->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].x += dx;
		m_poly->setVertices(vertices);
		m_poly->refresh();
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_poly->getRect().yCenter();
		std::vector<Vector> vertices = m_poly->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].y += dy;
		m_poly->setVertices(vertices);
		m_poly->refresh();
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float x = m_poly->getRect().xCenter();
			std::vector<Vector> vertices = m_poly->getVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].x = x * 2 - vertices[i].x;
			m_poly->setVertices(vertices);
			m_poly->refresh();
		}
		else if (type == 2)
		{
			float y = m_poly->getRect().yCenter();
			std::vector<Vector> vertices = m_poly->getVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].y = y * 2 - vertices[i].y;
			m_poly->setVertices(vertices);
			m_poly->refresh();
		}
	}

	m_editPanel->Refresh();
}

void PolygonPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void PolygonPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_poly->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_poly->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_poly->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		static const wxChar* mirror_labels[] = { wxT("none"),
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Mirror"))->Enable(bEnable);
}

} // d2d