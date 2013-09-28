#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class ISprite;

	class SpritePropertySetting : public IPropertySetting
	{
	public:
		SpritePropertySetting(EditPanel* editPanel, ISprite* sprite);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		ISprite* m_sprite;

	}; // SpritePropertySetting
}

