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

#ifndef D2D_FONT_H
#define D2D_FONT_H

#include "common/ResourcesMgr.h"

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>

namespace d2d
{
	class Font;
	typedef ResourcesMgr<Font> FontMgr;

	// from nehe's FreeType
	class Font
	{
	public:
		Font();
		virtual ~Font();

		bool loadFromFile(const wxString& filepath);

		void setSize(unsigned int h);

	private:
		void make_dlist(char ch);

	public:
		unsigned int h;			///< Holds the height of the font.
		unsigned int * textures;	///< Holds the texture id's 
		unsigned int list_base;	///< Holds the first display list id

		FT_Face face;

	}; // Font
}

#endif // D2D_FONT_H