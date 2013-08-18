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

#ifndef D2D_ANIM_SYMBOL_H
#define D2D_ANIM_SYMBOL_H

#include <vector>

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class AnimSymbol : public ISymbol
	{
	public:
		struct Frame
		{
			int index;
			std::vector<ISprite*> sprites;
		};

		struct Layer
		{
			std::vector<Frame*> frames;
		};

	public:
		AnimSymbol();
		virtual ~AnimSymbol();

		//
		// ICloneable interface
		//
		virtual AnimSymbol* clone() { return NULL; }

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

		size_t getMaxFrameIndex() const;

	protected:
		virtual void loadResources();

	private:
		void initBounding();

		void refreshThumbnail();

	public:
		std::vector<Layer*> m_layers;

		Rect m_rect;

		int m_fps;

	}; // AnimSymbol
}

#endif // D2D_ANIM_SYMBOL_H