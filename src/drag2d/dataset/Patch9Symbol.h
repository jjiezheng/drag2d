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

#ifndef D2D_9PATCH_SYMBOL_H
#define D2D_9PATCH_SYMBOL_H

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class Patch9Symbol : public ISymbol
	{
	public:
		Patch9Symbol();
		virtual ~Patch9Symbol();

		//
		// ICloneable interface
		//
		virtual Patch9Symbol* clone() { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		void composeFromSprites(ISprite* sprites[3][3], 
			float width, float height);

		void resize(float width, float height);

	protected:
		virtual void loadResources();

	private:
		void initBounding();

		void refreshThumbnail();

		void composeFromSprites();

		static void stretch(ISprite* sprite, const d2d::Vector& center, 
			float width, float height);

	public:
		// 2
		// 1
		// 0 1 2
		ISprite* m_sprites[3][3];

		float m_width, m_height;

	}; // Patch9Symbol
}

#endif // D2D_9PATCH_SYMBOL_H