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

#include "ArrangeSpriteAtomicOP.h"

#include "common/Math.h"
#include "view/MultiSpritesImpl.h"

using namespace d2d;
using namespace d2d::arrange_sprite;

//////////////////////////////////////////////////////////////////////////
// class SpritesAOP
//////////////////////////////////////////////////////////////////////////

SpritesAOP::SpritesAOP(const SpriteSelection& selection)
{
	selection.traverse(FetchAllVisitor<ISprite>(m_sprites));
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->retain();
}

SpritesAOP::SpritesAOP(const std::vector<ISprite*>& sprites)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		sprites[i]->retain();
		m_sprites.push_back(sprites[i]);
	}
}

SpritesAOP::~SpritesAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
}

//////////////////////////////////////////////////////////////////////////
// class MoveSpritesAOP
//////////////////////////////////////////////////////////////////////////

MoveSpritesAOP::MoveSpritesAOP(const SpriteSelection& selection, const Vector& offset)
	: SpritesAOP(selection)
{
	m_offset = offset;
}

MoveSpritesAOP::MoveSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& offset)
	: SpritesAOP(sprites)
{
	m_offset = offset;
}

void MoveSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->translate(-m_offset);
	}
}

void MoveSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->translate(m_offset);
	}
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpritesAOP
//////////////////////////////////////////////////////////////////////////

RotateSpritesAOP::RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end)
	: SpritesAOP(selection)
	, m_start(start)
	, m_end(end)
{
}

RotateSpritesAOP::RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end)
	: SpritesAOP(sprites)
	, m_start(start)
	, m_end(end)
{
}

void RotateSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
		sprite->rotate(-angle);
	}
}

void RotateSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
		sprite->rotate(angle);
	}
}

//////////////////////////////////////////////////////////////////////////
// class DeleteSpritesAOP
//////////////////////////////////////////////////////////////////////////

DeleteSpritesAOP::DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl)
	: SpritesAOP(selection)
{
	m_spritesImpl = spritesImpl;
}

DeleteSpritesAOP::DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl)
	: SpritesAOP(sprites)
{
	m_spritesImpl = spritesImpl;
}

void DeleteSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_spritesImpl->insertSprite(m_sprites[i]);
}

void DeleteSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_spritesImpl->removeSprite(m_sprites[i]);
}
