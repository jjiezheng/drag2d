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

#include "PropertySettingPanel.h"
#include "IPropertySetting.h"

using namespace d2d;

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxWindow(parent, wxID_ANY)
	, m_setting(NULL)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	m_pg = new wxPropertyGrid(
		this, 
		-1,
		wxDefaultPosition,
		wxSize(400, 800),
		wxPG_SPLITTER_AUTO_CENTER/* | wxPG_BOLD_MODIFIED*/
		);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertySettingPanel::onPropertyGridChange));

	sizer->Add(m_pg);

	SetSizer(sizer);	
}

PropertySettingPanel::~PropertySettingPanel()
{
	delete m_setting;
}

void PropertySettingPanel::setPropertySetting(IPropertySetting* setting)
{
	delete m_setting;
	m_setting = setting;

	if (m_setting)
		m_setting->updatePanel(this);
	else
		m_pg->Clear();
}

void PropertySettingPanel::onPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	m_setting->onPropertyGridChange(property->GetName(), property->GetValue());
}

void PropertySettingPanel::updatePropertyGrid()
{
	if (m_setting)
		m_setting->updatePropertyGrid(this);
}

void PropertySettingPanel::enablePropertyGrid(bool bEnable)
{
	if (m_setting)
		m_setting->enablePropertyGrid(this, bEnable);
}
