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

#ifndef D2D_ARRANGE_SPRITE_OP_CPP_
#define D2D_ARRANGE_SPRITE_OP_CPP_

#include "ArrangeSpriteOP.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/AbstractBV.h"
#include "common/Math.h"
#include "common/visitors.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "history/ArrangeSpriteAtomicOP.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{
	template <typename TBase>
	const float ArrangeSpriteOP<TBase>::SCALE_NODE_RADIUS = 2.0f;

	template <typename TBase>
	ArrangeSpriteOP<TBase>::ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel/* = NULL*/, AbstractEditCMPT* callback/* = NULL*/, bool scaleOpen/* = true*/)
		: TBase(editPanel, spritesImpl, propertyPanel, callback)
		, m_scaleOpen(scaleOpen)
		, m_spritesImpl(spritesImpl)
		, m_propertyPanel(propertyPanel)
		, m_bRightPress(false)
		, m_scaling(NULL)
		, m_bDirty(false)
		, m_autoAlignOpen(false)
	{
		m_selection = spritesImpl->getSpriteSelection();
		m_selection->retain();

		m_firstPos.setInvalid();

		m_scaleSelected.setInvalid();
	}

	template <typename TBase>
	ArrangeSpriteOP<TBase>::~ArrangeSpriteOP()
	{
		m_selection->release();
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onKeyDown(int keyCode)
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			sprites[i]->retain();

		if (TBase::onKeyDown(keyCode)) 
		{
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
				sprites[i]->release();
			return true;
		}

		switch (keyCode)
		{
		case WXK_DELETE:
			{
				std::vector<ISprite*> noPhysicsSprites;
				for (size_t i = 0, n = sprites.size(); i < n; ++i)
					if (!sprites[i]->getBody()) 
						noPhysicsSprites.push_back(sprites[i]);
				m_editPanel->addHistoryOP(new arrange_sprite::DeleteSpritesAOP(noPhysicsSprites, m_spritesImpl));
			}
			m_spritesImpl->removeSpriteSelection();
			m_scaling = NULL;
			break;
		case 'a': case 'A':
			translateSprite(Vector(-1, 0));
			break;
		case 'd': case 'D':
			translateSprite(Vector(1, 0));
			break;
		case 's': case 'S':
			translateSprite(Vector(0, -1));
			break;
		case 'w': case 'W':
			translateSprite(Vector(0, 1));
			break;
		}

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			sprites[i]->release();
		return false;
	}

	// todo: ugly
	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onMouseLeftDown(int x, int y)
	{
		m_lastPos = m_editPanel->transPosScreenToProject(x, y);
		m_scaleSelected.setInvalid();

		if (m_scaleOpen && m_scaling)
		{
			Vector quad[4];
			const float xscale = m_scaling->getScaleX(),
				yscale = m_scaling->getScaleY();
			Math::computeQuadNodes(m_scaling->getPosition(), m_scaling->getAngle(), 
				xscale, yscale, m_scaling->getBounding()->width() / xscale, 
				m_scaling->getBounding()->height() / yscale, quad);

			for (size_t i = 0; i < 4; ++i)
			{
				if (Math::getDistance(quad[i], m_lastPos) < SCALE_NODE_RADIUS)
				{
					m_scaleSelected = m_scaling->getPosition() + (quad[i] - m_scaling->getPosition()) / m_scaling->getScaleX();
					return false;
				}
			}
		}

		if (TBase::onMouseLeftDown(x, y)) return true;

		setScalingFromSelected();

		if (!m_selection->empty())
		{
			m_firstPos = m_lastPos;
		}
		m_bRightPress = false;

		m_autoAlignOpen = false;
		m_autoAlignHor[0] = m_autoAlignHor[1];
		m_autoAlignVer[0] = m_autoAlignVer[1];

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onMouseLeftUp(int x, int y)
	{
		if (TBase::onMouseLeftUp(x, y)) return true;

		setScalingFromSelected();

		if (m_firstPos.isValid() && !m_selection->empty() && !m_bRightPress)
		{
			Vector pos = m_editPanel->transPosScreenToProject(x, y);
			m_editPanel->addHistoryOP(new arrange_sprite::MoveSpritesAOP(*m_selection, pos - m_firstPos));
			m_firstPos.setInvalid();
		}

		if (m_propertyPanel && m_bDirty)
		{
			m_propertyPanel->enablePropertyGrid(true);
			m_propertyPanel->updatePropertyGrid();
			m_bDirty = false;
		}

		if (m_autoAlignOpen && !m_selection->empty())
		{
			std::vector<ISprite*> sprites;
			m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
			autoAlign(sprites);
		}

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onMouseRightDown(int x, int y)
	{
		if (TBase::onMouseRightDown(x, y)) return true;

		m_lastPos = m_editPanel->transPosScreenToProject(x, y);
		if (m_selection->size() == 1)
		{
			m_bRightPress = true;
			m_firstPos = m_lastPos;
		}

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onMouseRightUp(int x, int y)
	{
		if (TBase::onMouseRightUp(x, y)) return true;

		if (m_bRightPress)
		{
			m_bRightPress = false;
			if (m_firstPos.isValid() && !m_selection->empty())
			{
				Vector curr = m_editPanel->transPosScreenToProject(x, y);
				d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(curr);
				if (curr == m_firstPos && sprite)
				{
					wxMenu menu;
					menu.Append(EditPanel::Menu_UpOneLayer, EditPanel::menu_entries[EditPanel::Menu_UpOneLayer]);
					menu.Append(EditPanel::Menu_DownOneLayer, EditPanel::menu_entries[EditPanel::Menu_DownOneLayer]);
					m_editPanel->PopupMenu(&menu, x, y);
				}
				else
				{
					Vector pos = m_editPanel->transPosScreenToProject(x, y);
					m_editPanel->addHistoryOP(new arrange_sprite::RotateSpritesAOP(*m_selection, m_firstPos, pos));
				}
			}
		}

		if (m_propertyPanel && m_bDirty)
		{
			m_propertyPanel->enablePropertyGrid(true);
			m_propertyPanel->updatePropertyGrid();
			m_bDirty = false;
		}

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onMouseDrag(int x, int y)
	{
		if (TBase::onMouseDrag(x, y)) return true;

		if (m_scaleOpen && m_scaling && m_scaleSelected.isValid())
		{
			Vector pos = m_editPanel->transPosScreenToProject(x, y);
			scaleSprite(pos);
			return false;
		}

		if (m_selection->empty()) return false;

		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		if (m_bRightPress)
			rotateSprite(pos);
		else
			translateSprite(pos - m_lastPos);
		m_lastPos = pos;

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onPopMenuSelected(int type)
	{
		if (TBase::onPopMenuSelected(type)) return true;

		switch (type)
		{
		case EditPanel::Menu_UpOneLayer:
			{
				d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(m_firstPos);
				m_spritesImpl->resetSpriteOrder(sprite, true);
			}
			break;
		case EditPanel::Menu_DownOneLayer:
			{
				d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(m_firstPos);
				m_spritesImpl->resetSpriteOrder(sprite, false);
			}
			break;
		}

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::onDraw() const
	{
		if (TBase::onDraw()) return true;

		if (m_scaleOpen && m_scaling)
		{
			const float xscale = m_scaling->getScaleX(),
				yscale = m_scaling->getScaleY();
			Vector quad[4];
			Math::computeQuadNodes(m_scaling->getPosition(), m_scaling->getAngle(), 
				xscale, yscale, m_scaling->getBounding()->width() / xscale, 
				m_scaling->getBounding()->height() / yscale, quad);

			for (size_t i = 0; i < 4; ++i)
				PrimitiveDraw::drawCircle(quad[i], SCALE_NODE_RADIUS, false, 2, Colorf(0.2f, 0.8f, 0.2f));
		}

		if (m_autoAlignHor[0] != m_autoAlignHor[1])
			PrimitiveDraw::drawDashLine(m_autoAlignHor[0], m_autoAlignHor[1], Colorf(0, 0, 0));
		if (m_autoAlignVer[0] != m_autoAlignVer[1])
			PrimitiveDraw::drawDashLine(m_autoAlignVer[0], m_autoAlignVer[1], Colorf(0, 0, 0));

		return false;
	}

	template <typename TBase>
	bool ArrangeSpriteOP<TBase>::clear()
	{
		if (TBase::clear()) return true;

		m_lastPos.setInvalid();
		m_bRightPress = false;
		m_scaling = NULL;

		return false;
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::translateSprite(const Vector& delta)
	{
		m_selection->traverse(TranslateVisitor(delta));
		if (!m_selection->empty()) 
		{
			m_autoAlignOpen = true;
			if (m_propertyPanel && !m_bDirty)
			{
				m_propertyPanel->enablePropertyGrid(false);
				m_bDirty = true;
			}
			m_editPanel->Refresh();
		}
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::rotateSprite(const Vector& dst)
	{
		if (m_selection->size() != 1) return;
		m_selection->traverse(RotateVisitor(m_lastPos, dst));
		if (m_propertyPanel && !m_bDirty)
		{
			m_propertyPanel->enablePropertyGrid(false);
			m_bDirty = true;
		}
		m_editPanel->Refresh();
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::scaleSprite(const Vector& currPos)
	{
		if (!m_scaleOpen) return;

		const Vector& center = m_scaling->getPosition();
		Vector foot;
		Math::getFootOfPerpendicular(center, m_scaleSelected, currPos, &foot);
		const float scale = Math::getDistance(foot, center) / Math::getDistance(m_scaleSelected, center);
		m_scaling->setScale(scale);
		if (m_propertyPanel && !m_bDirty)
		{
			m_propertyPanel->enablePropertyGrid(false);
			m_bDirty = true;
		}
		m_editPanel->Refresh();
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::setScalingFromSelected()
	{
		if (!m_scaleOpen) return;

		bool refresh = false;
		if (m_selection->size() == 1)
		{
			refresh = m_scaling == NULL;
			std::vector<ISprite*> sprites;
			m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
			m_scaling = sprites[0];
		}
		else
		{
			refresh = m_scaling != NULL;
			m_scaling = NULL;
		}

		if (refresh) 
			m_editPanel->Refresh();
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::autoAlign(const std::vector<ISprite*>& sprites)
	{
		m_autoAlignHor[0].set(0, 0);
		m_autoAlignHor[1].set(0, 0);
		m_autoAlignVer[0].set(0, 0);
		m_autoAlignVer[1].set(0, 0);

		std::vector<ISprite*> sources;
		m_spritesImpl->traverseSprites(FetchAllVisitor<ISprite>(sources));

		// not support multi src now
		if (sprites.size() > 1)
			return;

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			for (size_t j = 0, m = sources.size(); j < m; ++j)
			{
				if (sprites[i] != sources[j])
					autoAlign(sources[j], sprites[i]);
			}
		}
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::autoAlign(const ISprite* src, ISprite* dst)
	{
		const float DIS = 5;
		const float LEN = 400;

		const float srcHalfWidth = src->getBounding()->width() * 0.5f,
			srcHalfHeight = src->getBounding()->height() * 0.5f;
		const float dstHalfWidth = dst->getBounding()->width() * 0.5f,
			dstHalfHeight = dst->getBounding()->height() * 0.5f;

		float srcLeft = src->getPosition().x - srcHalfWidth;
		float srcRight = src->getPosition().x + srcHalfWidth;
		float srcDown = src->getPosition().y - srcHalfHeight;
		float srcUp = src->getPosition().y + srcHalfHeight;

		// up
		float nearest = DIS;
		float dstUp = dst->getPosition().y + dstHalfHeight;
		if (float dis = fabs(dstUp - srcUp) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(dst->getPosition().x, srcUp - dstHalfHeight), 
				dst->getAngle());
			m_autoAlignHor[0].set(dst->getPosition().x - LEN, srcUp);
			m_autoAlignHor[1].set(dst->getPosition().x + LEN, srcUp);
		}
		else if (float dis = fabs(dstUp - srcDown) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(dst->getPosition().x, srcDown - dstHalfHeight), 
				dst->getAngle());
			m_autoAlignHor[0].set(dst->getPosition().x - LEN, srcDown);
			m_autoAlignHor[1].set(dst->getPosition().x + LEN, srcDown);
		}		
		// down
		float dstDown = dst->getPosition().y - dstHalfHeight;
		if (float dis = fabs(dstDown - srcUp) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(dst->getPosition().x, srcUp + dstHalfHeight), 
				dst->getAngle());
			m_autoAlignHor[0].set(dst->getPosition().x - LEN, srcUp);
			m_autoAlignHor[1].set(dst->getPosition().x + LEN, srcUp);
		}
		else if (float dis = fabs(dstDown - srcDown) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(dst->getPosition().x, srcDown + dstHalfHeight), 
				dst->getAngle());
			m_autoAlignHor[0].set(dst->getPosition().x - LEN, srcDown);
			m_autoAlignHor[1].set(dst->getPosition().x + LEN, srcDown);
		}	
		// left
		nearest = DIS;
		float dstLeft = dst->getPosition().x - dstHalfWidth;
		if (float dis = fabs(dstLeft - srcLeft) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(srcLeft + dstHalfWidth, dst->getPosition().y), 
				dst->getAngle());
			m_autoAlignVer[0].set(srcLeft, dst->getPosition().y - LEN);
			m_autoAlignVer[1].set(srcLeft, dst->getPosition().y + LEN);
		}
		else if (float dis = fabs(dstLeft - srcRight) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(srcRight + dstHalfWidth, dst->getPosition().y),
				dst->getAngle());
			m_autoAlignVer[0].set(srcRight, dst->getPosition().y - LEN);
			m_autoAlignVer[1].set(srcRight, dst->getPosition().y + LEN);
		}
		// right
		float dstRight = dst->getPosition().x + dstHalfWidth;
		if (float dis = fabs(dstRight - srcLeft) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(srcLeft - dstHalfWidth, dst->getPosition().y), 
				dst->getAngle());
			m_autoAlignVer[0].set(srcLeft, dst->getPosition().y - LEN);
			m_autoAlignVer[1].set(srcLeft, dst->getPosition().y + LEN);
		}
		else if (float dis = fabs(dstRight - srcRight) < nearest)
		{
			nearest = dis;
			dst->setTransform(Vector(srcRight - dstHalfWidth, dst->getPosition().y),
				dst->getAngle());
			m_autoAlignVer[0].set(srcRight, dst->getPosition().y - LEN);
			m_autoAlignVer[1].set(srcRight, dst->getPosition().y + LEN);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// class ArrangeSpriteOP<TBase>::TranslateVisitor
	//////////////////////////////////////////////////////////////////////////

	template <typename TBase>
	ArrangeSpriteOP<TBase>::TranslateVisitor::
		TranslateVisitor(const Vector& delta)
		: m_delta(delta)
	{
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::TranslateVisitor::
		visit(ICloneable* object, bool& bFetchNext)
	{
		ISprite* sprite = static_cast<ISprite*>(object);
		sprite->translate(m_delta);
		bFetchNext = true;
	}

	//////////////////////////////////////////////////////////////////////////
	// class ArrangeSpriteOP<TBase>::RotateVisitor
	//////////////////////////////////////////////////////////////////////////

	template <typename TBase>
	ArrangeSpriteOP<TBase>::RotateVisitor::
		RotateVisitor(const Vector& start, const Vector& end)
		: m_start(start), m_end(end)
	{
	}

	template <typename TBase>
	void ArrangeSpriteOP<TBase>::RotateVisitor::
		visit(ICloneable* object, bool& bFetchNext)
	{
		ISprite* sprite = static_cast<ISprite*>(object);

		float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
		sprite->rotate(angle);

		bFetchNext = false;
	}
}

#endif // D2D_ARRANGE_SPRITE_OP_CPP_
