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

#ifndef D2D_MULTI_SPRITES_IMPL_H
#define D2D_MULTI_SPRITES_IMPL_H

#include <wx/wx.h>

#include "EditPanelDefs.h"

#include "common/visitors.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiSpritesImpl
	{
	public:
		MultiSpritesImpl(wxWindow* wnd);
		virtual ~MultiSpritesImpl();

		virtual void traverseSprites(IVisitor& visitor, 
			TraverseType type = e_allExisting, bool order = true) const = 0;
		virtual void removeSprite(ISprite* sprite) = 0;
		virtual void insertSprite(ISprite* sprite) = 0;
		virtual void clear() = 0;

		virtual ISprite* querySpriteByPos(const Vector& pos) const;
		virtual void querySpritesByRect(const Rect& rect, std::vector<ISprite*>& result) const;		

		SpriteSelection* getSpriteSelection() { return m_spriteSelection; }
		void removeSpriteSelection();

	private:
		class PointQueryVisitor : public IVisitor
		{
		public:
			PointQueryVisitor(const Vector& pos, ISprite** pResult);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			ISprite** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, std::vector<ISprite*>& result);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			std::vector<ISprite*>& m_result;

		}; // RectQueryVisitor

		class RemoveSelectionVisitor : public IVisitor
		{
		public:
			RemoveSelectionVisitor(MultiSpritesImpl* spritesImpl);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			MultiSpritesImpl* m_spritesImpl;

		}; // RemoveSelectionVisitor

	protected:
		wxWindow* m_wnd;

		SpriteSelection* m_spriteSelection;

	}; // MultiSpritesImpl
}

#endif // D2D_MULTI_SPRITES_IMPL_H
