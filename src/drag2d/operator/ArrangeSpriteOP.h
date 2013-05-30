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

#ifndef D2D_ARRANGE_SPRITE_OP_H
#define D2D_ARRANGE_SPRITE_OP_H

#include "dataset/SelectionSet.h"

namespace d2d
{
	class EditPanel;
	class MultiSpritesImpl;
	class PropertySettingPanel;
	class AbstractEditCMPT;

	template <typename TBase>
	class ArrangeSpriteOP : public TBase
	{
	public:
		ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL,
			bool scaleOpen = true);
		virtual ~ArrangeSpriteOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	protected:
		virtual void translateSprite(const Vector& delta);
		virtual void rotateSprite(const Vector& dst);
		virtual void scaleSprite(const Vector& currPos);

	private:
		void setScalingFromSelected();

	private:
		class TranslateVisitor : public IVisitor
		{
		public:
			TranslateVisitor(const Vector& delta);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector& m_delta;

		}; // TranslateVisitor

		class RotateVisitor : public IVisitor
		{
		public:
			RotateVisitor(const Vector& start, const Vector& end);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector &m_start, &m_end;

		}; // RotateVisitor

	private:
		static const float SCALE_NODE_RADIUS;

	private:
		MultiSpritesImpl* m_spritesImpl;

		PropertySettingPanel* m_propertyPanel;

	protected:
		SpriteSelection* m_selection;
		Vector m_lastPos;

		ISprite* m_scaling;

	private:
		bool m_scaleOpen;

		bool m_bRightPress;

		Vector m_firstPos;

		Vector m_scaleSelected;

		bool m_bDirty;

	}; // ArrangeSpriteOP
}

#include "ArrangeSpriteOP.cpp"

#endif // D2D_ARRANGE_SPRITE_OP_H
