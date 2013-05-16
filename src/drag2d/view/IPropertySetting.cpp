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

#include "IPropertySetting.h"
#include "PropertySettingPanel.h"

using namespace d2d;

const wxString IPropertySetting::TYPE_NAME = wxT("ÀàÐÍ");

IPropertySetting::IPropertySetting(d2d::EditPanel* editPanel,
								   const wxString& type)
	: m_editPanel(editPanel)
	, m_type(type)
{
}

wxString IPropertySetting::getPGType(wxPropertyGrid* pg) const
{
	if (!pg) return wxEmptyString;
	wxPGProperty* property = pg->GetProperty(TYPE_NAME);
	if (!property) return wxEmptyString;
	wxAny value = property->GetValue();
	return wxANY_AS(value, wxString);
}
