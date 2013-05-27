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

#ifndef D2D_TEXT_SPRITE_H
#define D2D_TEXT_SPRITE_H

#include "ISprite.h"

#include "FontSymbol.h"

#include "common/Color.h"

namespace d2d
{
	class TextSprite : public ISprite
	{
	public:
		TextSprite();
		TextSprite(const TextSprite& sprite);
		TextSprite(FontSymbol* symbol);

		//
		// IObject interface
		//
		virtual TextSprite* clone();

		//
		// ISprite interface
		//
		virtual const FontSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();
		virtual void setTransform(const Vector& position, float angle);

		void setText(const std::string& text);
		const std::string& getText() const {
			return m_text;
		}

		void setSize(int size);
		int getSize() const { return m_size; }

		void setColor(const d2d::Colori& color) { m_color = color; }
		const d2d::Colori& getColor() const { return m_color; }

	protected:
		virtual void buildBounding();

	protected:
		FontSymbol* m_symbol;

	private:
		std::string m_text;

		int m_size;

		d2d::Colori m_color;

	}; // TextSprite
}

#endif // D2D_TEXT_SPRITE_H

