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


#include "SpritesPanelImpl.h"
#include "LibraryPanel.h"
#include "EditPanel.h"

#include "dataset/SpriteFactory.h"

using namespace d2d;

SpritesPanelImpl::SpritesPanelImpl(EditPanel* editPanel, LibraryPanel* libraryPanel)
	: MultiSpritesImpl(editPanel)
{
	m_editPanel = editPanel;
	m_libraryPanel = libraryPanel;
	editPanel->SetDropTarget(new DragSymbolTarget(this));
}

SpritesPanelImpl::~SpritesPanelImpl()
{
	clearSprites();
}

void SpritesPanelImpl::traverseSprites(IVisitor& visitor, TraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	traverseSprites(m_sprites, visitor, type, order);
}

void SpritesPanelImpl::removeSprite(ISprite* sprite)
{
	removeSprite(m_sprites, sprite);
}

void SpritesPanelImpl::insertSprite(ISprite* sprite)
{
	insertSprite(m_sprites, sprite);
	m_wnd->Refresh();
}

void SpritesPanelImpl::clearSprites()
{
	clearSprites(m_sprites);
}

void SpritesPanelImpl::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	resetSpriteOrder(m_sprites, sprite, up);
}

void SpritesPanelImpl::traverseSprites(const std::vector<d2d::ISprite*>& sprites, 
									   IVisitor& visitor, TraverseType type/* = e_allExisting*/,
									   bool order/* = true*/)
{
	if (order)
	{
		std::vector<ISprite*>::const_iterator itr = sprites.begin();
		for ( ; itr != sprites.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
	else
	{
		std::vector<ISprite*>::const_reverse_iterator itr = sprites.rbegin();
		for ( ; itr != sprites.rend(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
}

void SpritesPanelImpl::removeSprite(std::vector<d2d::ISprite*>& sprites, ISprite* sprite)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		if (sprites[i] == sprite)
		{
			sprites[i]->release();
			sprites.erase(sprites.begin() + i);
			break;
		}
	}
}

void SpritesPanelImpl::insertSprite(std::vector<d2d::ISprite*>& sprites, ISprite* sprite)
{
	sprite->retain();
	sprites.push_back(sprite);
}

void SpritesPanelImpl::clearSprites(std::vector<d2d::ISprite*>& sprites)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->release();
	sprites.clear();
}

void SpritesPanelImpl::resetSpriteOrder(std::vector<d2d::ISprite*>& sprites, 
										d2d::ISprite* sprite, bool up)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		if (sprites[i] == sprite)
		{
			if (up && i != n - 1)
			{
				ISprite* tmp = sprites[i];
				sprites[i] = sprites[i+1];
				sprites[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				ISprite* tmp = sprites[i];
				sprites[i] = sprites[i-1];
				sprites[i-1] = tmp;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class SpritesPanelImpl::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

SpritesPanelImpl::DragSymbolTarget::
DragSymbolTarget(SpritesPanelImpl* panelImpl)
{
	m_panelImpl = panelImpl;
}

bool SpritesPanelImpl::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ISymbol* symbol = m_panelImpl->m_libraryPanel->getSymbol(index);
	if (symbol)
	{
		Vector pos = m_panelImpl->m_editPanel->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::create(symbol);
		sprite->translate(pos);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return true;
}
