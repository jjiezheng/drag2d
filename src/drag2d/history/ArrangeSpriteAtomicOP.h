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

#ifndef D2D_ARRANGE_SPRITE_ATOMIC_OP_H
#define D2D_ARRANGE_SPRITE_ATOMIC_OP_H

#include <vector>

#include "AbstractAtomicOP.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiSpritesImpl;

	namespace arrange_sprite
	{
		class SpritesAOP : public AbstractAtomicOP
		{
		public:
			SpritesAOP(const SpriteSelection& selection);
			SpritesAOP(const std::vector<ISprite*>& sprites);
			virtual ~SpritesAOP();

		protected:
			std::vector<ISprite*> m_sprites;

		}; // SpritesAOP

		class MoveSpritesAOP : public SpritesAOP
		{
		public:
			MoveSpritesAOP(const SpriteSelection& selection, const Vector& offset);
			MoveSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& offset);

			virtual void undo();
			virtual void redo();

		private:
			Vector m_offset;

		}; // MoveSpritesAOP

		class RotateSpritesAOP : public SpritesAOP
		{
		public:
			RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end);
			RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end);

			virtual void undo();
			virtual void redo();

		private:
			Vector m_start, m_end;

		}; // RotateSpritesAOP

		class DeleteSpritesAOP : public SpritesAOP
		{
		public:
			DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl);
			DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl);

			virtual void undo();
			virtual void redo();

		private:
			MultiSpritesImpl* m_spritesImpl;

		}; // DeleteSpritesAOP
	}
}

#endif // D2D_ARRANGE_SPRITE_ATOMIC_OP_H
