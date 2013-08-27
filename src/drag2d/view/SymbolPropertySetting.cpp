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

#include "SymbolPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/ISymbol.h"
#include "view/EditPanel.h"

using namespace d2d;

SymbolPropertySetting::SymbolPropertySetting(EditPanel* editPanel, ISymbol* symbol)
	: IPropertySetting(editPanel, wxT("ISymbol"))
	, m_symbol(symbol)
{
}

void SymbolPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_symbol->name);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
	}
}

void SymbolPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_symbol->name = wxANY_AS(value, wxString);

	m_editPanel->Refresh();
}

void SymbolPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void SymbolPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
}