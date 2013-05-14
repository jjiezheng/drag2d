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

#include "MultiSpritesImpl.h"

using namespace d2d;

MultiSpritesImpl::MultiSpritesImpl(wxWindow* wnd)
{
	m_wnd = wnd;
	m_spriteSelection = new SpriteSelection;
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_spriteSelection->release();
}

ISprite* MultiSpritesImpl::querySpriteByPos(const Vector& pos) const
{
	ISprite* result = NULL;
	traverseSprites(PointQueryVisitor(pos, &result), e_editable, false);
	return result;
}

void MultiSpritesImpl::querySpritesByRect(const Rect& rect, std::vector<ISprite*>& result) const
{
	traverseSprites(RectQueryVisitor(rect, result), e_editable);
}

void MultiSpritesImpl::removeSpriteSelection()
{
	if (!m_spriteSelection->empty())
	{
		m_spriteSelection->traverse(RemoveSelectionVisitor(this));
		m_spriteSelection->clear();
		m_wnd->Refresh();
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::PointQueryVisitor::PointQueryVisitor(const Vector& pos, ISprite** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void MultiSpritesImpl::PointQueryVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->isContain(m_pos))
	{
		*m_pResult = sprite;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::RectQueryVisitor::RectQueryVisitor(const Rect& rect, std::vector<ISprite*>& result)
	: m_rect(rect), m_result(result)
{
}

void MultiSpritesImpl::RectQueryVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->isIntersect(m_rect))
		m_result.push_back(sprite);
	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RemoveSelectionVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::RemoveSelectionVisitor::RemoveSelectionVisitor(MultiSpritesImpl* spritesImpl)
{
	m_spritesImpl = spritesImpl;
}

void MultiSpritesImpl::RemoveSelectionVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	m_spritesImpl->removeSprite(static_cast<ISprite*>(object));
	bFetchNext = true;
}
