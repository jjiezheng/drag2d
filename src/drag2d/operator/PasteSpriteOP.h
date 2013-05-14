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

#ifndef D2D_PASTE_SPRITE_OP_H
#define D2D_PASTE_SPRITE_OP_H

#include <vector>

#include "SelectSpritesOP.h"

#include "dataset/SelectionSet.h"

namespace d2d
{
	class ISprite;
	class SpritesPanelImpl;
	class PasteSpriteCMPT;

	class PasteSpriteOP : public SelectSpritesOP
	{
	public:
		PasteSpriteOP(EditPanel* editPanel, SpritesPanelImpl* spritesImpl,
			PropertySettingPanel* propertyPanel, PasteSpriteCMPT* cmpt = NULL);
		virtual ~PasteSpriteOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		void setMousePos(int x, int y);
		void fixPosOrthogonal();

	private:
		class SpriteBatch
		{
		public:
			SpriteBatch();
			~SpriteBatch();

			void loadFromSelection(const SpriteSelection& selection);

			void insertToSpritesImpl(SpritesPanelImpl* spritesImpl, const Vector& pos,
				bool isHorMirror, bool isVerMirror);
			void draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const;

			const Vector& getCenter() const { return m_center; }

			void clear();

			bool empty() const { return m_selected.empty(); }

		private:
			void computeCenter();

		private:
			std::vector<ISprite*> m_selected;
			Vector m_center;

		}; // SpriteBatch

	private:
		SpritesPanelImpl* m_spritesImpl;

		PasteSpriteCMPT* m_cmpt;

		SpriteSelection* m_selection;

		Vector m_pos;

		SpriteBatch m_batch;

	}; // PasteSpriteOP
}

#endif // D2D_PASTE_SPRITE_OP_H
