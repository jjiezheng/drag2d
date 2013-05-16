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

#ifndef D2D_RAW_PIXELS_H
#define D2D_RAW_PIXELS_H

#include <set>

namespace d2d
{
	class RawPixels
	{
	public:
		RawPixels();
		~RawPixels();

 		void loadFromFile(const char* filepath);
// 		unsigned int storeToTexture(unsigned int old) const;

		const unsigned char* getPixelData() const;
		void getSize(int& width, int& height) const;
		int getChannels() const;

	public:
		struct PixelsPos
		{
			int x, y;
		}; // PixelsPos

		class PixelCmp
		{
		public:
			bool operator () (const PixelsPos* lhs, const PixelsPos* rhs) const {
				return lhs->x < rhs->x
					|| lhs->x == rhs->x && lhs->y < rhs->y;
			}
		}; // PixelCmp

		typedef std::set<PixelsPos*, PixelCmp> PixelBuf;

		void replaceBySelected(const PixelBuf& selected);

	private:
		unsigned char* m_pixelData;
		int m_width, m_height, m_channels;

	}; // RawPixels
}

#endif // D2D_RAW_PIXELS_H
