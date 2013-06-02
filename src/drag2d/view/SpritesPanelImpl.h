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

#ifndef D2D_SPRITES_PANEL_IMPL_H
#define D2D_SPRITES_PANEL_IMPL_H

#include "MultiSpritesImpl.h"

#include <wx/dnd.h>

namespace d2d
{
	class EditPanel;
	class LibraryPanel;
	class ISprite;

	class SpritesPanelImpl : public MultiSpritesImpl
	{
	public:
		SpritesPanelImpl(EditPanel* editPanel, LibraryPanel* libraryPanel);
		virtual ~SpritesPanelImpl();

		virtual void traverseSprites(IVisitor& visitor, 
			TraverseType type = e_allExisting, bool order = true) const;
		virtual void removeSprite(ISprite* sprite);
		virtual void insertSprite(ISprite* sprite);
		virtual void clearSprites();

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(SpritesPanelImpl* panelImpl);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			SpritesPanelImpl* m_panelImpl;

		}; // DragSymbolTarget

	protected:
		LibraryPanel* m_libraryPanel;

	private:
		EditPanel* m_editPanel;

		std::vector<ISprite*> m_sprites;

		friend class DragSymbolTarget;

	}; // SpritesPanelImpl
}

#endif // D2D_SPRITES_PANEL_IMPL_H
