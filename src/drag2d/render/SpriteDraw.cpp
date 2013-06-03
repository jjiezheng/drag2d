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

#include "SpriteDraw.h"
#include "GL10.h"
#include "SpriteBatch.h"

#include "common/Math.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/ImageSprite.h"

using namespace d2d;

void SpriteDraw::drawSprite(const ISprite* sprite)
{
	GL10::PushMatrix();

	GL10::Translatef(sprite->getPosition().x, sprite->getPosition().y, 0.0f);

	GL10::Rotatef(sprite->getAngle() * TRANS_RAD_TO_DEG, 0, 0, 1);

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	const float xScale = xMirror ? -sprite->getScale() : sprite->getScale(),
		yScale = yMirror ? -sprite->getScale() : sprite->getScale();
	GL10::Scalef(xScale, yScale, 1.0f);

	sprite->getSymbol().draw(sprite);

	GL10::PopMatrix();
}

void SpriteDraw::drawSprite(const ISymbol* symbol, const Vector& pos,
							float angle/* = 0.0f*/, float scale/* = 1.0f*/)
{
	GL10::PushMatrix();
	GL10::Translatef(pos.x, pos.y, 0.0f);
	GL10::Scalef(scale, scale, 1.0f);
	GL10::Rotatef(angle * TRANS_RAD_TO_DEG, 0, 0, 1);
	symbol->draw();
	GL10::PopMatrix();
}

void SpriteDraw::drawSprites(const std::vector<ISprite*>& sprites,
							 SpriteBatch& batch)
{
	GL10::Enable(GL10::GL_BLEND);
	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	batch.clear();
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
// 		ImageSprite* sprite = dynamic_cast<ImageSprite*>(sprites[i]);
// 		if (sprite)
// 			batch.add(sprite);
// 		else
			SpriteDraw::drawSprite(static_cast<ISprite*>(sprites[i]));
	}
	batch.onDraw();

	GL10::Disable(GL10::GL_BLEND);
}

void SpriteDraw::begin(const ISprite* sprite)
{
	GL10::PushMatrix();
	GL10::Translatef(sprite->getPosition().x, sprite->getPosition().y, 0.0f);
	GL10::Rotatef(sprite->getAngle() * TRANS_RAD_TO_DEG, 0, 0, 1);
}

void SpriteDraw::end(const ISprite* sprite)
{
	GL10::PopMatrix();
}
