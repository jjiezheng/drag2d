#pragma once

#include <vector>

#include "DrawRectangleOP.h"

#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiSpritesImpl;
	class AbstractEditCMPT;
	class PropertySettingPanel;
	class IPropertySetting;

	class SelectSpritesOP : public DrawRectangleOP
	{
	public:
		SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback = NULL,
			const Colorf& color = Colorf(0, 0, 0));
		virtual ~SelectSpritesOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		virtual IPropertySetting* createPropertySetting(ISprite* sprite) const;
		virtual IPropertySetting* createPropertySetting(const std::vector<ISprite*>& sprites) const;

	protected:
		SpriteSelection* m_selection;

		PropertySettingPanel* m_propertyPanel;

		MultiSpritesImpl* m_spritesImpl;

		AbstractEditCMPT* m_callback;

	private:
		void clearClipboard();

	private:
		Vector m_firstPos;

		std::vector<ISprite*> m_clipboard;
		bool m_lastCtrlPress;

		// To disable mouse able when press ctrl and window query
		bool m_bDraggable;

	}; // SelectSpritesOP
}

