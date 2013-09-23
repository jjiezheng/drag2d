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

#ifndef D2D_SYMBOL_PROPERTY_SETTING_H
#define D2D_SYMBOL_PROPERTY_SETTING_H

#include "IPropertySetting.h"

namespace d2d
{
	class ISymbol;

	class SymbolPropertySetting : public IPropertySetting
	{
	public:
		SymbolPropertySetting(EditPanel* editPanel, ISymbol* symbol);
		SymbolPropertySetting(EditPanel* editPanel, std::string* name);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		ISymbol* m_symbol;

		std::string* m_name;

	}; // SymbolPropertySetting
}

#endif // D2D_SYMBOL_PROPERTY_SETTING_H