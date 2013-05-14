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

#include "DrawSpritesVisitor.h"
#include "SpriteBatch.h"
#include "SpriteDraw.h"
#include "GL10.h"

#include "dataset/ImageSprite.h"

using namespace d2d;

DrawSpritesVisitor::DrawSpritesVisitor(SpriteBatch& batch)
	: m_batch(batch)
{
	GL10::Enable(GL10::GL_BLEND);
	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	m_batch.clear();
}

DrawSpritesVisitor::~DrawSpritesVisitor()
{
	m_batch.onDraw();

	GL10::Disable(GL10::GL_BLEND);
}

void DrawSpritesVisitor::visit(ICloneable* object, bool& bFetchNext)
{
	bFetchNext = true;

// 	ImageSprite* sprite = dynamic_cast<ImageSprite*>(object);
// 	if (sprite)
// 		m_batch.add(sprite);
// 	else
		SpriteDraw::drawSprite(static_cast<ISprite*>(object));
}
