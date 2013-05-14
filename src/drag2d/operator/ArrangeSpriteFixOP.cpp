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

#include "ArrangeSpriteFixOP.h"

#include "view/MultiSpritesImpl.h"

using namespace d2d;

ArrangeSpriteFixOP::ArrangeSpriteFixOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback/* = NULL*/)
	: ArrangeSpriteOP(editPanel, spritesImpl, propertyPanel, callback)
	, m_spritesImpl(spritesImpl)
{
}

bool ArrangeSpriteFixOP::onMouseLeftUp(int x, int y)
{
	if (ArrangeSpriteOP::onMouseLeftUp(x, y)) return true;

	std::vector<ISprite*> sprites;
	m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
	fixSpritesLocation(sprites);

	return false;
}

bool ArrangeSpriteFixOP::onMouseRightDown(int x, int y)
{
	if (ArrangeSpriteOP::onMouseRightDown(x, y)) return true;

	onMouseLeftDown(x, y);

	return false;
}

bool ArrangeSpriteFixOP::onMouseRightUp(int x, int y)
{
	return onMouseLeftUp(x, y);
}

void ArrangeSpriteFixOP::fixSpritesLocation(const std::vector<ISprite*>& sprites)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->retain();

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->removeSprite(sprites[i]);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->insertSprite(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->release();
}
