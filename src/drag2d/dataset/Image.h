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

#ifndef D2D_IMAGE_H
#define D2D_IMAGE_H

#include "common/ResourcesMgr.h"
#include "common/Rect.h"

#include <wx/wx.h>

namespace d2d
{
	class Image;
	typedef ResourcesMgr<Image> ImageMgr;

	class Image
	{
	public:
		Image();
		
		bool loadFromFile(const wxString& filepath);
		void reload() const;

		const wxString& filepath() const { return m_filepath; }

		unsigned int textureID() const { return m_textureID; }
		int width() const { return m_width; }
		int height() const { return m_height; }

		void draw() const;

		const Rect& getRegion() const { return m_region; }

	private:
		void removeTransparentBorder();
		bool isTransparent(unsigned char* pixels, int x, int y, int channels);

	private:
		wxString m_filepath;

		mutable unsigned int m_textureID;
		int m_width, m_height;
		Rect m_region;

	}; // Image
}

#endif // D2D_IMAGE_H
