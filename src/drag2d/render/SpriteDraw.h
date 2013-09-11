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

#ifndef D2D_SPRITE_DRAW_H
#define D2D_SPRITE_DRAW_H

#include <vector>

class wxMemoryDC;

namespace d2d
{
	class ISprite;
	class ISymbol;
	class SpriteBatch;
	class Vector;

	class SpriteDraw
	{
	public:
		static void drawSprite(const ISprite* sprite);

		static void drawSprite(const ISymbol* symbol, const Vector& pos,
			float angle = 0.0f, float xScale = 1.0f, float yScale = 1.0f);

		static void drawSprite(const ISprite* sprite, wxMemoryDC& memDC);
		static void drawSprite(const ISprite* sprite, const Vector& offset, wxMemoryDC& memDC,
			float bgwidth = 800, float bgheight = 480, float scale = 0.15f);

		static void drawSprites(const std::vector<ISprite*>& sprites, 
			SpriteBatch& batch);

		static void begin(const ISprite* sprite);
		static void end(const ISprite* sprite);

	public:
		// for AnimateSymbol
		static float time;

	}; // SpriteDraw
}

#endif // D2D_SPRITE_DRAW_H
