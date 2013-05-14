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

#ifndef D2D_COMBINATION_SYMBOL_H
#define D2D_COMBINATION_SYMBOL_H

#include <vector>

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class ISprite;

	class CombinationSymbol : public ISymbol
	{
	public:
		CombinationSymbol();
		CombinationSymbol(const std::vector<ISprite*>& children);
		virtual ~CombinationSymbol();

		//
		// IObject interface
		//	
		virtual CombinationSymbol* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw() const;
		virtual float getWidth() const;
		virtual float getHeight() const;

 		const std::vector<ISprite*>& getChildren() const {
 			return m_sprites;
 		}
		const Rect& getRect() const { return m_rect; }

	protected:
		virtual void loadResources();

	private:
		void moveToCenter();

		void initBounding();

	private:
		std::vector<ISprite*> m_sprites;

		Rect m_rect;

	}; // CombinationSymbol
}

#endif // D2D_COMBINATION_SYMBOL_H
